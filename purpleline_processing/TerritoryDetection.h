#ifndef TERRITORYDETECTION_H
#define TERRITORYDETECTION_H

#include "ColorDetection.h"
#include "ImageUtils.h"
#include "GridMap.h"
#include "CameraMath.h"

namespace TerritoryDetection {

void detectPurpleLine(cv::Mat& frame, GridMap& local_map);
void detectPurpleLineTest(cv::Mat& frame, GridMap& local_map);
void detectPurpleLineTest2(cv::Mat& frame, GridMap& local_map);
std::vector<cv::Vec4i> removeSimilarLines(std::vector<cv::Vec4i> lines, double threshold);
double findCrossProduct(cv::Vec4i v1, cv::Vec4i v2);
double findLength(cv::Vec4i v1);
}

#endif // TERRITORYDETECTION_H
