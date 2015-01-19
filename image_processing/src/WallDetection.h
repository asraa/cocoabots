#ifndef WALLDETECTION_H
#define WALLDETECTION_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <math.h>

#include "ImageUtils.h"
#include "ColorDetection.h"
#include "GridMap.h"
#include "CameraMath.h"

namespace WallDetection {

static const int COLOR_WHITE = 255;
static const int COLOR_BLACK = 0; // for drawing map (??)

static const int WALL_LINE_THRESH = 10; // vertical threshold for wall

static const int FIELD_OBJECT_BLUE_WALL = 1; // to label map

static const double WALL_LINE_WIDTH = 2;
static const double WALL_HEIGHT = 6; // white + line
static const double WALL_HEIGHT_TO_LINE = 4;

void detectWall(cv::Mat&, GridMap&, int); // highest level code

cv::Mat getWallLine(cv::Mat&, int);
int * scanLine(cv::Mat&, cv::Mat&);

void updateMapPts(GridMap&, int *, int);
void updateMapPts(GridMap&, int, double [], int); // from scanline to update map

}


#endif // WALLDETECTION_H
