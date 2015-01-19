#ifndef BLOCKDETECTION_H
#define BLOCKDETECTION_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <math.h>

#include "CameraMath.h"
#include "ContourUtils.h"
#include "ColorDetection.h"
#include "ImageUtils.h"

namespace BlockDetection {


static const double BLOCK_HEIGHT = 2; // inches

// for crude estimate
static const double BLOCK_THRESH = 10;
static const double COS_THRESH = 0.1;

static const double POLY_NEIGHBORHOOD = 7;

Eigen::Vector2d crudeEstimate(std::vector<cv::Point>&);

void detectBlock(cv::Mat&);

int numOfBlocksEst(std::vector<cv::Point> &);


}

#endif // BLOCKDETECTION_H
