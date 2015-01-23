#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <cmath>

#include "CameraConfig.h"


namespace ImageUtils {

void binaryImagePreProcess(cv::Mat&, int);

cv::Mat cannyEdge(cv::Mat&);

}

#endif // IMAGEUTILS_H
