#ifndef COLORDETECTION_H
#define COLORDETECTION_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>

#include "CameraConfig.h"

namespace ColorDetection {

static const int COLOR_LINE_BLUE = 1;
static const int COLOR_LINE_RED = 2;
static const int COLOR_LINE_YELLOW = 3;
static const int COLOR_WALL = 4;
static const int COLOR_BLOCK_RED = 5;
static const int COLOR_BLOCK_GREEN = 6;

cv::Mat detectColor(cv::Mat&,int);
cv::Mat detectColor(cv::Mat&,int[],int);


}

#endif // COLORDETECTION_H


/*
 *
 detectwall(){
    what's involved in here?
    - get blue color -- ColorDetection
    - canny edges/contour
    - check if is wall (if using contour)
    - search line by line -- function that does this?? sure.
    - remove pixels above -- in the function above
    - determine x,y coords in camera coord
    -- do the same for yellow
    */
