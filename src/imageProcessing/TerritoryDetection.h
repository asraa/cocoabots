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


void detectPurpleLine(cv::Mat& frame, GridMap& local_map);

}

#endif // TERRITORYDETECTION_H
