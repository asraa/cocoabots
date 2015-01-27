#include "particlefilter.h"
#include <random>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <unistd.h>

#include "../configFile.h"

//initialize a vector with the default standard non normalized probabilities (all zeros)
const std::vector<double> particleFilter::initialProbabilities(PARTICLE_FILTER_INITIAL_NUMBER_OF_PARTICLES, 1.0);


particleFilter::particleFilter(double positionX, double positionY):
    myProbabilities(initialProbabilities),
    mySensors(NULL),
    myMap(NULL)
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

    for(int i=0;i<PARTICLE_FILTER_INITIAL_NUMBER_OF_PARTICLES;i++){
        x = normalDistributionX(randomNumberGenerator);
        y = normalDistributionY(randomNumberGenerator);
        angle = uniformDistributionAngle(randomNumberGenerator);

        particle.x=x;
        particle.y=y;
        particle.angle=angle;

        myParticles.push_back(particle);
    }
        numberOfParticles=PARTICLE_FILTER_INITIAL_NUMBER_OF_PARTICLES;

}


particleFilter::particleFilter(double positionX,
                               double positionY,
                               sensorsModule *sensorsPtr,
                               map *mapPtr):particleFilter(positionX,positionY){
    mySensors = sensorsPtr;
    myMap = mapPtr;
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
    int resampledCounter=1;
    double previousPosition;
    double previousAngle;
    double distance;
    double differenceAngle;
    double position;
    double angle;
    int stillUpdates=PARTICLE_FILTER_MAX_STILL_UPDATES;
    int moved=0;
    previousPosition = myParticleFilter->getNewPosition();
    previousAngle = myParticleFilter->getNewAngle();

    while (myParticleFilter->running) {
        position=myParticleFilter->getNewPosition();
        angle=myParticleFilter->getNewAngle();
        moved=0;
        if (position!=previousPosition || angle!=previousAngle){
            moved=1;
            distance = position - previousPosition;
            differenceAngle = angle - previousAngle;
            stillUpdates=PARTICLE_FILTER_MAX_STILL_UPDATES;
            myParticleFilter->updateParticles(differenceAngle,distance);
            previousPosition = position;
            previousAngle=angle;
            updatedPositionCounter++;
        }
        if(moved||stillUpdates==0){
            stillUpdates--;
            if(myParticleFilter->myMap)
                myParticleFilter->updateProbabilities();

            if(!(updatedPositionCounter%PARTICLE_FILTER_UPDATE_RESAMPLE_RATIO)){
                updatedPositionCounter=0;
                myParticleFilter->resample();
                resampledCounter++;


            }
        }
        if(resampledCounter%PARTICLE_FILTER_RESAMPLE_RESPAWN_RATIO==0){
            resampledCounter%=PARTICLE_FILTER_RESAMPLE_RESPAWN_RATIO;
            resampledCounter++;
            myParticleFilter->respawn();
        }
        myParticleFilter->updateRobotPosition();
        usleep(PARTICLE_FILTER_UPDATE_RATE_MS*1000);
    }
}

void particleFilter::respawn(){
    if(numberOfParticles<PARTICLE_FILTER_NUMBER_OF_PARTICLES_RESPAWN){
        struct particleFilterParticle particle;
        double x;
        double y;
        double angle;
        std::default_random_engine randomNumberGenerator;
        std::normal_distribution<double> normalDistributionX(robot.x,PARTICLE_FILTER_STD_DEVIATION_X_RESPAWN);
        std::normal_distribution<double> normalDistributionY(robot.y, PARTICLE_FILTER_STD_DEVIATION_Y_RESPAWN);
        std::normal_distribution<double> normalDistributionAngle(robot.angle, PARTICLE_FILTER_STD_DEVIATION_ANGLEE_RESPAWN);

        for(int i=0;i<PARTICLE_FILTER_NUMBER_OF_PARTICLES_RESPAWN;i++){
            x = normalDistributionX(randomNumberGenerator);
            y = normalDistributionY(randomNumberGenerator);
            angle = normalDistributionAngle(randomNumberGenerator);

            particle.x=x;
            particle.y=y;
            particle.angle=angle;

            myParticles.push_back(particle);
        }
        numberOfParticles+=PARTICLE_FILTER_NUMBER_OF_PARTICLES_RESPAWN;

    }
}

void particleFilter::killParticles(){
    double maxLikelyhood=0;
    std::vector <struct particleFilterParticle> newParticles;
    std::vector <double> newProbabilities;
    for(int i=0  ; i<numberOfParticles;i++){
        if(i< myProbabilities.size()){
            if(myProbabilities[i]>maxLikelyhood){
                maxLikelyhood=myProbabilities[i];
            }
        }else{
            printf("%d  index ERROR!", i);
        }
    }
    for(int i=0; i<numberOfParticles;i++){
        if(i< myProbabilities.size()){

            if(myProbabilities[i]>PARTICLE_FILTER_KILL_PROBABILITY_THRESHOLD*maxLikelyhood){
                newProbabilities.push_back(myProbabilities[i]);
                newParticles.push_back(myParticles[i]);
            }
        }else{
            printf("%d  index ERROR!", i);
        }
    }
    numberOfParticles=newParticles.size();
    myParticles=newParticles;
    myProbabilities=newProbabilities;

}

void particleFilter::updateProbabilities(){
    std::vector <struct particleFilterParticle> tempParticles = myParticles;// protects against race conditions, etc
    struct particleFilterParticle *particlePtr;
    int numberOfParticles = tempParticles.size();

    double likelyhood;
    double realFrontReading = 0;
    double realBackReading = 0;
    double realRightReading = 0;
    double realLeftReading = 0;

#if PARTICLE_FILTER_FRONT == 1
    realFrontReading=mySensors->frontShortIRData;
#endif

#if PARTICLE_FILTER_FRONT ==2
    realFrontReading=mySensors->frontUltrasonicData;
#endif

#if PARTICLE_FILTER_BACK == 1
    realBackReading=mySensors->backShortIRData;
#endif

#if PARTICLE_FILTER_BACK ==2
    realBackReading=mySensors->backUltrasonicData;
#endif

#if PARTICLE_FILTER_RIGHT == 1
    realRightReading=mySensors->rightShortIRData;
#endif

#if PARTICLE_FILTER_RIGHT ==2
    realRightReading=mySensors->rightUltrasonicData;
#endif


#if PARTICLE_FILTER_LEFT == 1
    realLeftReading=mySensors->leftShortIRData;
#endif

#if PARTICLE_FILTER_LEFT ==2
    realLeftReading=mySensors->leftUltrasonicData;
#endif

    for (int i=0; i<numberOfParticles; i++){
        particlePtr=&tempParticles[i];
    //TODO GET EXPECTED SENSORS READINGS FROM POSITION OF THE PARTICLE
    // get sensor reading(particlePtr->x,particlePtr->y,particlePtr->angle)
        double x = particlePtr->x;
        double y = particlePtr->y;
        double angle = particlePtr->angle;
        double frontReading = myMap->getSonarReadingFront(x,y,angle);
        double backReading = myMap->getSonarReadingBack(x,y,angle);
        double rightReading = myMap->getSonarReadingRight(x,y,angle);
        double leftReading  = myMap->getSonarReadingLeft(x,y,angle);




        //TODO Compare the expected reading with the real reading and see the probability
        // Use: probability = normalPdf(value, median(value of the sensor), standardDeviationOfTheSensor))
        likelyhood=1;
#if PARTICLE_FILTER_FRONT == 1
        if (frontReading<PARTICLE_FILTER_MAX_IR_RANGE){
            likelyhood*=normalPdf(realFrontReading,frontReading,PARTICLE_FILTER_STANDARD_DEVIATION_IR);
        }
#endif

#if PARTICLE_FILTER_FRONT ==2
        if (frontReading<PARTICLE_FILTER_MAX_ULTRASONIC_RANGE){
            likelyhood*=normalPdf(realFrontReading,frontReading,PARTICLE_FILTER_STANDARD_DEVIATION_ULTRASONIC);

        }
#endif

#if PARTICLE_FILTER_RIGHT == 1
        if (frontReading<PARTICLE_FILTER_MAX_IR_RANGE){
            likelyhood*=normalPdf(realRightReading,rightReading,PARTICLE_FILTER_STANDARD_DEVIATION_IR);
        }
#endif

#if PARTICLE_FILTER_RIGHT ==2
        if (frontReading<PARTICLE_FILTER_MAX_ULTRASONIC_RANGE){
            likelyhood*=normalPdf(realRightReading,rightReading,PARTICLE_FILTER_STANDARD_DEVIATION_ULTRASONIC);

        }
#endif
#if PARTICLE_FILTER_LEFT == 1
        if (frontReading<PARTICLE_FILTER_MAX_IR_RANGE){
            likelyhood*=normalPdf(realLeftReading,leftReading,PARTICLE_FILTER_STANDARD_DEVIATION_IR);
        }
#endif

#if PARTICLE_FILTER_LEFT ==2
        if (frontReading<PARTICLE_FILTER_MAX_ULTRASONIC_RANGE){
            likelyhood*=normalPdf(realLeftReading,leftReading,PARTICLE_FILTER_STANDARD_DEVIATION_ULTRASONIC);

        }
#endif
#if PARTICLE_FILTER_BACK == 1
        if (backReading<PARTICLE_FILTER_MAX_IR_RANGE){
            likelyhood*=normalPdf(realBackReading,backReading,PARTICLE_FILTER_STANDARD_DEVIATION_IR);
        }
#endif

#if PARTICLE_FILTER_BACK ==2
        if (frontReading<PARTICLE_FILTER_MAX_ULTRASONIC_RANGE){
            likelyhood*=normalPdf(realBackReading,backReading,PARTICLE_FILTER_STANDARD_DEVIATION_ULTRASONIC);

        }
#endif
        //TODO multiply the probability of the particle by this probability
        myProbabilities[i]*=likelyhood;
    }



}

struct particleFilterParticle particleFilter::updateRobotPosition(){
    std::vector <struct particleFilterParticle> tempParticles = myParticles;// protects against race conditions, etc
    std::vector <double> tempProbabilities = myProbabilities;
    int numberOfParticles = tempParticles.size();
    double probabilityNormalizationFactor=0;
    double x=0.0;
    double y=0.0;
    double angle=0.0;
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

struct particleFilterParticle updateRobotVariance(){

}

void particleFilter::resample(){
    std::default_random_engine randomNumberGenerator; //This is used to get
                                                      //particles from the distribution


    //(2) range constructor
    //The sequence of values in the range is used as the weights for each integer value
    //from 0 to (n-1), where n is the distance between the iterators.
    //As seen in http://www.cplusplus.com/reference/random/discrete_distribution/discrete_distribution/

    killParticles();
    std::discrete_distribution<int> probabilityDistribution(myProbabilities.begin(),
                                                       myProbabilities.end());


    std::vector <struct particleFilterParticle> newDistributionOfParticles;
    int index;
    struct particleFilterParticle particle;
    for(int i=0;i<numberOfParticles;i++){
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

    for(int i=0;i<numberOfParticles;i++){
        particle = &(myParticles[i]);

        noiseDistance = noiseDistanceDistribution(randomNumberGenerator);
        noiseAngle = noiseAngleDistribution(randomNumberGenerator);

        newDistance = noiseDistance;
        newAngle = noiseAngle + particle->angle;
        movingAngle = (newAngle+particle->angle) / 2 * PARTICLE_FILTER_ANGLE_NEGATIVE_CLOCKWISE; //Some geometry involved in this
        newDistanceX = cos(movingAngle*PI/180) * newDistance;
        newDistanceY = sin(movingAngle*PI/180) * newDistance;

        if(!myMap->isWall(particle->x+newDistanceX, particle->y+newDistanceY)){
            particle->angle = newAngle;
            particle->x += newDistanceX;
            particle->y += newDistanceY;
        }
    }
}

double particleFilter::getNewAngle(){
    return mySensors->getAngle();
}


double particleFilter::getNewPosition(){
    return mySensors->getPosition();
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
///
void particleFilter::createSimpleWebpageView(std::string nameOfFile){
    createSimpleWebpageView(nameOfFile, "map.png");
}

void particleFilter::createSimpleWebpageView(std::string nameOfFile, std::string nameOfMapFile){
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
           << "canvas.width = 800; \n"
           << "canvas.height = 600; \n"
           << "var context = canvas.getContext('2d');\n"
           << "document.body.appendChild(canvas);\n" << std::endl;

    { //map
    std::string nameOfImage = "map";
    webpage << "var "<< nameOfImage<< " = new Image();\n"
      <<nameOfImage<<".src = '"<<nameOfMapFile<<"'; \n " << std::endl;
    }

    //particles
    for (int i=0; i<numberOfParticles;i++){
        std::string nameOfImage = "image";
        nameOfImage.append(std::to_string(i));

        webpage << "var "<< nameOfImage<< " = new Image();\n"
          <<nameOfImage<<".src = 'smallArrow.gif'; \n " << std::endl;
    }

    {//robot
    std::string nameOfImage = "robot";
    webpage << "var "<< nameOfImage<< " = new Image();\n"
      <<nameOfImage<<".src = 'smallRedArrow.gif'; \n " << std::endl;
    }

    webpage << "function init() {\n "<< std::endl;

    //robot
    {
        std::string nameOfImage = "map";
        webpage <<"context.drawImage("<<nameOfImage<<", 0, 0);\n"<< std::endl;

    }

    for (int i=0; i<numberOfParticles;i++){
        std::string nameOfImage = "image";
        nameOfImage.append(std::to_string(i));
        particle = tempParticles[i];
        int x = (int) (particle.x * PARTICLE_FILTER_INCHES_PIXEL_RATIO);
        int y = (int) (particle.y * PARTICLE_FILTER_INCHES_PIXEL_RATIO);
        y = PARTICLE_FILTER_MAP_Y_PIXELS-y;
        double angle = particle.angle/180*PI;

        webpage << "context.save();\n" << std::endl;

        //centralizes the image.
        webpage<< "context.translate(8, 8);\n"
              // << "context.translate(320, 240);\n"
               << std::endl;

        webpage << "context.translate(" << x <<","<< y <<");\n" <<
                   "context.rotate("<<angle*-1*PARTICLE_FILTER_ANGLE_NEGATIVE_CLOCKWISE<<");\n" << std::endl;


        webpage <<"context.drawImage("<<nameOfImage<<", -8, -8);\n"<< std::endl;

        webpage <<"context.restore();\n"<< std::endl;
    }

{
    std::string nameOfImage = "robot";

    particle = robot;
    int x = (int) particle.x * PARTICLE_FILTER_INCHES_PIXEL_RATIO;
    int y = (int) particle.y * PARTICLE_FILTER_INCHES_PIXEL_RATIO;
    y = PARTICLE_FILTER_MAP_Y_PIXELS-y;
    double angle = particle.angle/180*PI;

    webpage << "context.save();\n" << std::endl;

    //centralizes the image.
    webpage<< "context.translate(11, 9);\n"
         //  << "context.translate(320, 240);\n"
           << std::endl;

    webpage << "context.translate(" << x <<","<< y <<");\n" <<
               "context.rotate("<<angle*-1*PARTICLE_FILTER_ANGLE_NEGATIVE_CLOCKWISE<<");\n" << std::endl;


    webpage <<"context.drawImage("<<nameOfImage<<", -11, -9);\n"<< std::endl;

    webpage <<"context.restore();\n"<< std::endl;
}

    webpage << "}\n\n"<<
               "</script>\n\n" <<
               "</body>\n"<<
               "</html>"<< std::endl;


    webpage.close();
}
