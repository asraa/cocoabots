#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <math.h>

namespace ImageUtils {

static const int SMOOTH_KERNEL = 3;
static const int MORPH_KERNEL = 3;

void binaryImagePreProcess(cv::Mat&, int);

static const int CANNY_THRESH_LOW = 5;
static const int CANNY_THRESH_UP = 15;
static const int CANNY_KERNEL = 3;

cv::Mat cannyEdge(cv::Mat&);


}

#endif // IMAGEUTILS_H
