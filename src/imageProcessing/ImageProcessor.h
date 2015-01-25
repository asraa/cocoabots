
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <cmath>
#include <thread>
#include <cstring>

#include "GridMap.h"
#include "ColorDetection.h"
#include "WallDetection.h"
#include "CameraMath.h"
#include "ImageUtils.h"
#include "BlockDetection.h"
#include "CameraConfig.h"

/***********************************************
 ***********IMAGE PROCESSING CLASS**************
 ***********************************************/


class ImageProcessor 
{
public:

    ImageProcessor();
    ~ImageProcessor();
    cv::VideoCapture vid_cap;

    GridMap local_map; // for now

    // update for other threads to get
    BlockDetection::BlockInfo nearestBlockInfo;
    BlockDetection::BlockInfo nearestBlockInfoPrevious;

    /*
    int foundCube;
    double nearestCubeAngle;
    double nearestCubeDist;
    int nearestCubeColor;
    */

    cv::Mat frame_raw;
    cv::Mat frame;

    static const int CUBE_COLOR_GREEN = 0;
    static const int CUBE_COLOR_RED = 1;

    int running;
    std::thread *runThread;

    double cpu_time; // for debug

    void detectWall(cv::Mat&);
    void detectBlocks(cv::Mat&);


    void local_map_refresh();

    void updateNearestBlockInfoAverage();

    void writeToFile(std::string fn);

    void doStuff();
    void clearCameraCache();


    int getFoundCube();
    double getNearestCubeDist();
    double getNearestCubeAngle();
    int getNearestCubeColor();
    double getCpuTime();

    static void run(ImageProcessor * ImageProcessorPointer);
};

#endif // IMAGEPROCESSOR_H
