#ifndef BLOCKDETECTION_H
#define BLOCKDETECTION_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <cmath>
#include <ctime>

#include "CameraConfig.h"
#include "CameraMath.h"
#include "ImageUtils.h"
#include "ColorDetection.h"

namespace BlockDetection {

struct BlockInfo {
    double found_cube;
    double nearest_cube_angle;
    double nearest_cube_dist;
    double nearest_cube_color;

    BlockInfo(): found_cube(0.0), nearest_cube_angle(0.0), nearest_cube_dist(0.0), nearest_cube_color(0.0) {}
};

void detectBlocks(cv::Mat&, BlockInfo& nearest_block_info);

Eigen::Vector2d crudeEstimate(std::vector<cv::Point>&);

int numOfBlocksEst(std::vector<cv::Point> &);

int findLowestPoint(std::vector<cv::Point>& contour);
int findHighestPoint(std::vector<cv::Point>& contour);

bool isBlock(std::vector<cv::Point>& contour);
bool numberOfVerticesReasonable(std::vector<cv::Point>& contour);
bool aspectRatioReasonable(std::vector<cv::Point>& contour);
bool perimeterRatio2large(std::vector<cv::Point>& contour);
bool contour2small(std::vector<cv::Point>& contour);
Eigen::Vector2d findNearestBlockInList(std::vector<Eigen::Vector2d,Eigen::aligned_allocator<Eigen::Vector2d>>& list_of_block_pts);

int isVertical(cv::Point pt1, cv::Point pt2);

int findLowestContour(ImageUtils::ContourData& contour_data);

void updateBlockFoundInfo(Eigen::Vector2d& block_coord_rad, int cube_color, BlockInfo& nearest_block_info);
void updateBlockNotFound(BlockInfo& nearest_block_info);

}

#endif // BLOCKDETECTION_H
