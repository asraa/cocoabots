
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <math.h>
#include <thread>

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

    /*
    int foundCube;
    double nearestCubeAngle;
    double nearestCubeDist;
    int nearestCubeColor;
    */

    static const int CUBE_COLOR_GREEN = 0;
    static const int CUBE_COLOR_RED = 1;

    int running;
    std::thread *runThread;

    void detectWall(cv::Mat&);
    void detectBlocks(cv::Mat&);

    void local_map_refresh();

    int getFoundCube();
    double getNearestCubeDist();
    double getNearestCubeAngle();
    int getNearestCubeColor();

    static void run(ImageProcessor * ImageProcessorPointer);
};

#endif // IMAGEPROCESSOR_H
