
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <math.h>

#include "GridMap.h"
#include "ColorDetection.h"
#include "WallDetection.h"
#include "CameraMath.h"
#include "ImageUtils.h"

/***********************************************
 ***********IMAGE PROCESSING CLASS**************
 **********************************************/


class ImageProcessor 
{
public:
  ImageProcessor();

  struct ContourData {
    cv::Mat drawing;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
  };

  cv::VideoCapture vid_cap;
  
  int FEATURE_AREA_THRESH;

  double WALL_LINE_WIDTH;

  int BLUE_LINE_TOLERANCE;

  GridMap local_map; // for now

  void detectWall(cv::Mat&);
  void detectBlocks(cv::Mat&);

  ContourData getContours(cv::Mat&);
  bool contour2small(std::vector<cv::Point>&);
  void cleanContour(ContourData&);

  void run(void);
};

#endif // IMAGEPROCESSOR_H
