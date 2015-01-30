#ifndef WALLDETECTION_H
#define WALLDETECTION_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <cmath>
#include <time.h>

#include "CameraConfig.h"
#include "ImageUtils.h"
#include "ColorDetection.h"
#include "GridMap.h"
#include "CameraMath.h"

namespace WallDetection {

static const int FIELD_OBJECT_BLUE_WALL = 1; // to label map

void detectWall(cv::Mat&, GridMap&); // highest level code

cv::Mat getWallLine(cv::Mat&);
int * scanLine(cv::Mat&, cv::Mat&);

void updateMapPts(GridMap&, int *, int);
void updateMapPts(GridMap&, int, double [], int); // from scanline to update map

}


#endif // WALLDETECTION_H
