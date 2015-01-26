#ifndef TERRITORYDETECTION_H
#define TERRITORYDETECTION_H

#include "ColorDetection.h"
#include "ImageUtils.h"
#include "GridMap.h"
#include "CameraMath.h"

namespace TerritoryDetection {

void detectPurpleLine(cv::Mat& frame, GridMap& local_map);

}

#endif // TERRITORYDETECTION_H
