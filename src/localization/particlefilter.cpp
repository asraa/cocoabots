#include "particlefilter.h"
#include <random>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <unistd.h>

#include "../configFile.h"

//initialize a vector with the default standard non normalized probabilities (all zeros)
const std::vector<double> particleFilter::initialProbabilities(PARTICLE_FILTER_NUMBER_OF_PARTICLES, 1.0);


particleFilter::particleFilter(double positionX, double positionY):
    myProbabilities(initialProbabilities),
    mySensors(NULL),
    myMotorsControl(NULL)
{
    std::default_random_engine randomNumberGenerator; //This is used to get the
                                                      //position state from the distribution

    std::uniform_real_distribution<double> uniformDistributionAngle(0,360);
    std::normal_distribution<double> normalDistributionX(positionX,PARTICLE_FILTER_INITIAL_DISTRIBUTION_STANDARD_DEVIATION_X);
    std::normal_distribution<double> normalDistributionY(positionY, PARTICLE_FILTER_INITIAL_DISTRIBUTION_STANDARD_DEVIATION_Y);

    struct particleFilterParticle particle;
    double x;
    double y;
    double angle;
    myParticles.clear(); //redundant, since vector was never initialized, but safer.

    for(int i=0;i<PARTICLE_FILTER_NUMBER_OF_PARTICLES;i++){
        x = normalDistributionX(randomNumberGenerator);
        y = normalDistributionY(randomNumberGenerator);
        angle = uniformDistributionAngle(randomNumberGenerator);

        particle.x=x;
        particle.y=y;
        particle.angle=angle;

        myParticles.push_back(particle);
    }

}


particleFilter::particleFilter(double positionX,
                               double positionY,
                               sensorsModule *sensorsPtr,
                               motorsControl *motorsPtr):particleFilter(positionX,positionY){
    mySensors = sensorsPtr;
    myMotorsControl = motorsPtr;
    running = 1;
    runThread = new  std::thread(run,this);

}

particleFilter::~particleFilter(){
    running=0;
    runThread->join();
    delete runThread;
}

void particleFilter::run(particleFilter *particleFilterPtr){
    particleFilter * myParticleFilter = particleFilterPtr;
    int updatedPositionCounter;
    double previousPosition;
    double previousAngle;
    double distance;
    double differenceAngle;

    previousPosition = myParticleFilter->getNewPosition();
    previousAngle = myParticleFilter->getNewAngle();

    while (myParticleFilter->running) {

        distance = myParticleFilter->getNewPosition() - previousPosition;
        differenceAngle = myParticleFilter->getNewAngle() - previousAngle;

        myParticleFilter->updateParticles(differenceAngle,distance);

        previousPosition = myParticleFilter->getNewPosition();
        previousAngle=myParticleFilter->getNewAngle();

        updatedPositionCounter++;

        myParticleFilter->updateProbabilities();

        if(!(updatedPositionCounter%PARTICLE_FILTER_UPDATE_RESAMPLE_RATIO)){
            updatedPositionCounter=0;
            myParticleFilter->resample();

        }
        myParticleFilter->updateRobotPosition();
        usleep(PARTICLE_FILTER_UPDATE_RATE_MS*1000);
    }
}


void particleFilter::updateProbabilities(){
    std::vector <struct particleFilterParticle> tempParticles = myParticles;// protects against race conditions, etc
    struct particleFilterParticle *particlePtr;
    int numberOfParticles = tempParticles.size();

    double probability;
    for (int i=0; i<numberOfParticles; i++){
        particlePtr=&tempParticles[i];
    //TODO GET EXPECTED SENSORS READINGS FROM POSITION OF THE PARTICLE
    // get sensor reading(particlePtr->x,particlePtr->y,particlePtr->angle)

    //TODO Compare the expected reading with the real reading and see the probability
    // Use: probability = normalPdf(value, median(value of the sensor), standardDeviationOfTheSensor))
    probability=1;
    //TODO multiply the probability of the particle by this probability
    myProbabilities[i]*=probability;
    }



}

struct particleFilterParticle particleFilter::updateRobotPosition(){
    std::vector <struct particleFilterParticle> tempParticles = myParticles;// protects against race conditions, etc
    std::vector <double> tempProbabilities = myProbabilities;
    int numberOfParticles = tempParticles.size();
    double probabilityNormalizationFactor=0;
    double x;
    double y;
    double angle;
    double tempProbability;
    struct particleFilterParticle tempParticle;

    for (int i=0; i<numberOfParticles; i++){
        tempProbability=tempProbabilities[i];
        tempParticle=tempParticles[i];

        probabilityNormalizationFactor+=tempProbability;
        x += tempParticle.x*tempProbability;
        y += tempParticle.y*tempProbability;
        angle += tempParticle.angle*tempProbability;
    }

    x = x/probabilityNormalizationFactor;
    y = y/probabilityNormalizationFactor;
    angle = angle/probabilityNormalizationFactor;

    tempParticle.x=x;
    tempParticle.y=y;
    tempParticle.angle=angle;
    robot=tempParticle;
    return tempParticle;
}

double particleFilter::getRobotX(){
    return robot.x;
}

double particleFilter::getRobotY(){
    return robot.y;
}

double particleFilter::getRobotAngle(){
    return robot.angle;
}

void particleFilter::resample(){
    std::default_random_engine randomNumberGenerator; //This is used to get
                                                      //particles from the distribution


    //(2) range constructor
    //The sequence of values in the range is used as the weights for each integer value
    //from 0 to (n-1), where n is the distance between the iterators.
    //As seen in http://www.cplusplus.com/reference/random/discrete_distribution/discrete_distribution/

    std::discrete_distribution<int> probabilityDistribution(myProbabilities.begin(),
                                                       myProbabilities.end());


    std::vector <struct particleFilterParticle> newDistributionOfParticles;
    int index;
    struct particleFilterParticle particle;
    for(int i=0;i<PARTICLE_FILTER_NUMBER_OF_PARTICLES;i++){
        index= probabilityDistribution(randomNumberGenerator);
        particle = myParticles[index];
        newDistributionOfParticles.push_back(particle);
    }
    myParticles=newDistributionOfParticles;
    resetProbabilities();

}


void particleFilter::resetProbabilities(){
    myProbabilities = initialProbabilities;
}



void particleFilter::updateParticles(double differenceAngle, double distance){
    std::default_random_engine randomNumberGenerator; //This is used to get the
                                                      //noise from the distribution

    double standardDeviationDistance = distance * PARTICLE_FILTER_ENCODERS_NORMALIZED_STANDARD_DEVIATION;
    double standardDeviationAngle = differenceAngle * PARTICLE_FILTER_ANGLE_NORMALIZED_STANDARD_DEVIATION;
    std::normal_distribution<double> noiseDistanceDistribution(distance, standardDeviationDistance);
    std::normal_distribution<double> noiseAngleDistribution(differenceAngle, standardDeviationAngle);

    double newAngle;
    double movingAngle;
    double newDistance;
    double newDistanceX;
    double newDistanceY;
    double noiseDistance;
    double noiseAngle;
    struct particleFilterParticle * particle; //Pointer to the particle

    for(int i=0;i<PARTICLE_FILTER_NUMBER_OF_PARTICLES;i++){
        particle = &(myParticles[i]);

        noiseDistance = noiseDistanceDistribution(randomNumberGenerator);
        noiseAngle = noiseAngleDistribution(randomNumberGenerator);

        newDistance = distance + noiseDistance;
        newAngle = differenceAngle + noiseAngle + particle->angle;
        movingAngle = (newAngle+particle->angle) / 2 * PARTICLE_FILTER_ANGLE_NEGATIVE_CLOCKWISE; //Some geometry involved in this
        newDistanceX = cos(movingAngle*PI/180) * newDistance;
        newDistanceY = sin(movingAngle*PI/180) * newDistance;

        particle->angle = newAngle;
        particle->x += newDistanceX;
        particle->y += newDistanceY;
    }
}

double particleFilter::getNewAngle(){
    return myMotorsControl->getNewAngle();
}


double particleFilter::getNewPosition(){
    return myMotorsControl->getNewPosition();
}


float particleFilter::normalPdf(float value, float median, float standardDeviation)
{
    static const float inv_sqrt_2pi = 0.3989422804014327;
    float a = (value - median) / standardDeviation;

    return inv_sqrt_2pi / standardDeviation * std::exp(-0.5f * a * a);
}

///
/// \brief particleFilter::createSimpleWebpageView
/// Creates a simple webpage to show the positions of the particles and the robot.
/// \param nameOfFile
/// name of the file that will be created and destroyed.
void particleFilter::createSimpleWebpageView(std::string nameOfFile){
    remove(nameOfFile.c_str());
    std::ofstream webpage(nameOfFile,std::ofstream::out |  std::ios_base::app);
    std::vector <struct particleFilterParticle> tempParticles = myParticles;
    struct particleFilterParticle particle;

    webpage <<"<!DOCTYPE html>\n"
           <<"<html> \n<body> \n "<<
             "<meta http-equiv=\"refresh\" content=\"1\" >\n <script>\n\n" << std::endl;

    webpage << "window.addEventListener(\"load\", init);\n\n" << std::endl;

    int numberOfParticles;
    if (tempParticles.size()>PARTICLE_FILTER_MAX_PARTICLES_WEBPAGE){
        numberOfParticles = PARTICLE_FILTER_MAX_PARTICLES_WEBPAGE;
    }
    else{
        numberOfParticles = tempParticles.size();
    }

    webpage << "var canvas = document.createElement('canvas');\n "
           << "canvas.width = 640; \n"
           << "canvas.height = 480; \n"
           << "var context = canvas.getContext('2d');\n"
           << "document.body.appendChild(canvas);\n" << std::endl;
    for (int i=0; i<numberOfParticles;i++){
        std::string nameOfImage = "image";
        nameOfImage.append(std::to_string(i));

        webpage << "var "<< nameOfImage<< " = new Image();\n"
          <<nameOfImage<<".src = 'smallArrow.gif'; \n " << std::endl;
    }

    webpage << "function init() {\n "<< std::endl;

    for (int i=0; i<numberOfParticles;i++){
        std::string nameOfImage = "image";
        nameOfImage.append(std::to_string(i));
        particle = tempParticles[i];
        int x = (int) particle.x;
        int y = (int) particle.y;
        double angle = particle.angle/180*PI;

        webpage << "context.save();\n" << std::endl;

        //centralizes the image.
        webpage<< "context.translate(8, 8);\n"
               << "context.translate(320, 240);\n" << std::endl;

        webpage << "context.translate(" << x <<","<< y <<");\n" <<
                   "context.rotate("<<angle<<");\n" << std::endl;


        webpage <<"context.drawImage("<<nameOfImage<<", -16, -16);\n"<< std::endl;

        webpage <<"context.restore();\n"<< std::endl;
    }

    webpage << "}\n\n"<<
               "</script>\n\n" <<
               "</body>\n"<<
               "</html>"<< std::endl;


    webpage.close();
}
