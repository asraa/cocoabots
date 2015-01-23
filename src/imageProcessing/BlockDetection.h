#ifndef BLOCKDETECTION_H
#define BLOCKDETECTION_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <math.h>
#include <time.h>

#include "CameraMath.h"
#include "ContourUtils.h"
#include "ColorDetection.h"
#include "ImageUtils.h"

namespace BlockDetection {

struct BlockInfo {
    int found_cube;
    double nearest_cube_angle;
    double nearest_cube_dist;
    int nearest_cube_color;

    BlockInfo(): found_cube(0), nearest_cube_angle(0), nearest_cube_dist(0), nearest_cube_color(0) {}
};

static const double BLOCK_HEIGHT = 2; // inches

// for crude estimate
static const double BLOCK_THRESH = 10;
static const double COS_THRESH = 0.1;

static const int POLY_VERTEX_NUM_THRESH = 8;

static const double POLY_NEIGHBORHOOD = 9;

static const double FEATURE_AREA_THRESH = 100;

static const double ASPECT_RATIO_LOW = 0.8;
static const double ASPECT_RATIO_UP = 4;


void detectBlocks(cv::Mat&, BlockInfo& nearest_block_info);

Eigen::Vector2d crudeEstimate(std::vector<cv::Point>&);

int numOfBlocksEst(std::vector<cv::Point> &);

int findLowestPoint(std::vector<cv::Point> contour);
int findHighestPoint(std::vector<cv::Point> contour);

bool isBlock(std::vector<cv::Point>& contour);
bool numberOfVerticesReasonable(std::vector<cv::Point>& contour);
bool aspectRatioReasonable(std::vector<cv::Point>& contour);
bool perimeterRatio2large(std::vector<cv::Point>& contour);
bool contour2small(std::vector<cv::Point>& contour);

int isVertical(cv::Point pt1, cv::Point pt2);

int findLowestContour(ContourUtils::ContourData& contour_data);

void updateBlockFoundInfo(Eigen::Vector2d block_coord_cam, int cube_color, BlockInfo& nearest_block_info);
void updateBlockNotFound(BlockInfo& nearest_block_info);

}

#endif // BLOCKDETECTION_H
