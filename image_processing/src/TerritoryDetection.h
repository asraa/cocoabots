#ifndef TERRITORYDETECTION_H
#define TERRITORYDETECTION_H

#include "ColorDetection.h"
#include "ImageUtils.h"
#include "GridMap.h"
#include "CameraMath.h"

namespace TerritoryDetection {

struct PurpleLineInfo {
    int detected_purple_line;
    double rotation_angle_purple_line;
    double dist_to_purple_line;
    double rotation_angle_for_alignment;

    PurpleLineInfo(): detected_purple_line(0), rotation_angle_purple_line(0.0), dist_to_purple_line(0.0),
        rotation_angle_for_alignment(0.0) {}
};

void addHoughLine(cv::Vec2f& line, cv::Mat& out, CvScalar color, int width, int factor);
void findEndpoints(cv::Vec2f& line, cv::Mat& out);
void addHoughLines(std::vector<cv::Vec2f>& lines, cv::Mat& out, CvScalar color, int width, int factor);
cv::Vec4i getVectorFromHoughLine(cv::Vec2f& line, int factor);
int getAngleBetweenHoughLines(cv::Vec2f v1, cv::Vec2f v2);
std::vector<cv::Vec2f> removeDuplicates(std::vector<cv::Vec2f> lines, double threshold=5);
void mainProcess(cv::Mat& src);

void detectPurpleLine(cv::Mat& frame, GridMap& local_map);

}

#endif // TERRITORYDETECTION_H
