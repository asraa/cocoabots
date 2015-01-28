/* The particles nd the probabilities of the particles are separated in order to make use
 *  of algorithms from the std library, without defining a custom iterator.
 *
 *  In an ideal situation I would create a custom iterator for the particles that
 *  returns their probabilities, and others for each of the state parameters
 *
 *  However, to minimize debug time of the iterator, I'm making the code a little more messy.
 *
 * A better approach for this particle filter could be to draw particles from a normal distribution
 * for each one of the parameters. Instead, I'm drawing particles from the discrete distribution of
 * previous particles. This can be problematic, since it largely reduces the variance of the particles.
 *
 *
 * The vector of probabilities and the vector of particles should ALWAYS have the same size.
 */

#ifndef PARTICLEFILTER_H
#define PARTICLEFILTER_H
#include <vector>
#include <string>
#include "../sensorsmodule.h"
#include"../mapping/map.h"
#include"../actuators/motorscontrol.h" //The motorscontrol keep track of the speed, because they need to
                                //be updated less often than the sensors.

struct particleFilterParticle{
    double x;
    double y;
    double angle;
    double varianceX;
    double varianceY;
    double varianceAngle;
    particleFilterParticle():x(0),y(0),angle(0),
    varianceAngle(0),varianceX(0),
    varianceY(0){}
};


class particleFilter
{
public:
    particleFilter(double positionX, double positionY);//Only for simple debugging.
    particleFilter(double positionX,
                   double positionY,
                   sensorsModule * sensorsPtr,
                   map * mapPtr = NULL);

    ~particleFilter();
    struct particleFilterParticle robot;
    double getRobotX();
    double getRobotY();
    double getRobotAngle();
    struct particleFilterParticle updateRobotPosition();
    struct particleFilterParticle updateRobotVariance();
    std::vector <struct particleFilterParticle> myParticles;
    std::vector <double> myProbabilities;
    void resample();
    void updateParticles(double differenceAngle, double distance);
    void updateProbabilities();
    void resetProbabilities();
    static const std::vector<double> initialProbabilities;
    void createSimpleWebpageView(std::string nameOfFile, std::string nameOfMapFile);
    void createSimpleWebpageView(std::string nameOfFile);
    float normalPdf(float value, float median, float standardDeviation);
    void respawn();
    void killParticles();

    int running;
    std::thread *runThread;
    double getNewAngle();
    double getNewPosition();
    int numberOfParticles;


    static void run(particleFilter * particleFilterPtr);
private:
    sensorsModule * mySensors;
    map * myMap;
};
#endif // PARTICLEFILTER_H
