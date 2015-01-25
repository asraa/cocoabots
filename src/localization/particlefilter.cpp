#include "particlefilter.h"
#include <random>
#include <cmath>
#include <cstdio>
#include <fstream>
#include "../configFile.h"

//initialize a vector with the default standard non normalized probabilities (all zeros)
const std::vector<double> particleFilter::initialProbabilities(PARTICLE_FILTER_NUMBER_OF_PARTICLES, 1.0);

particleFilter::particleFilter(double positionX, double positionY):
    myProbabilities(initialProbabilities)
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
