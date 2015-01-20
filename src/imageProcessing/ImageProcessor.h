
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <math.h>

#include "GridMap.h"
#include "ColorDetection.h"
#include "WallDetection.h"
#include "CameraMath.h"
#include "ImageUtils.h"
#include "BlockDetection.h"

/***********************************************
 ***********IMAGE PROCESSING CLASS**************
 **********************************************/


class ImageProcessor 
{
public:
    ImageProcessor();

    cv::VideoCapture vid_cap;

    GridMap local_map; // for now

    static const int CUBE_COLOR_GREEN = 0;
    static const int CUBE_COLOR_RED = 1;

    // update for other threads to get
    int foundCube;
    double nearestCubeAngle;
    double nearestCubeDist;
    int nearestCubeColor;

    void detectWall(cv::Mat&);
    void detectBlocks(cv::Mat&);

    ContourData getContours(cv::Mat&);
    bool contour2small(std::vector<cv::Point>&);
    void cleanContour(ContourData&);

    void local_map_refresh();

    int getFoundCube();
    double getNearestCubeDist();
    double getNearestCubeAngle();
    int getNearestCubeColor();

    void run();
};

#endif // IMAGEPROCESSOR_H
