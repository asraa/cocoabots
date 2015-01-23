#ifndef CONTOURUTILS_H
#define CONTOURUTILS_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>

namespace ContourUtils {

struct ContourData {
   // cv::Mat drawing;
    std::vector<std::vector<cv::Point> > contours;
   // std::vector<cv::Vec4i> hierarchy;
};

ContourData getContours(cv::Mat&);
//void cleanContour(ContourData&);
//bool contour2small(std::vector<cv::Point>&);

//static const double FEATURE_AREA_THRESH = 0;

cv::Mat drawContours(ContourData &, cv::Mat&);

}

#endif // CONTOURUTILS_H
