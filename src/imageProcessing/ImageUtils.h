#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <cmath>

#include "CameraConfig.h"


namespace ImageUtils {

// gaussian blur and morphology operation
void binaryImagePreProcess(cv::Mat&, int);

// ******** CANNY EDGE ********** //
cv::Mat cannyEdge(cv::Mat&);

// ******* CONTOUR STUFF ********** //
struct ContourData {
   // cv::Mat drawing;
    std::vector<std::vector<cv::Point> > contours;
   // std::vector<cv::Vec4i> hierarchy;
};
ContourData getContours(cv::Mat& im_src);
cv::Mat drawContours(ContourData & contour_data, cv::Mat& im_src);
cv::Mat drawContoursFilled(ContourData& contour_data, cv::Mat& im_src);
cv::Mat getContoursMat(cv::Mat& im_src);
void cleanContour(ContourData& contour_data, double area);

void replaceByPolyContours(ImageUtils::ContourData& contour_data);

// ********** HOUGH LINE STUFF ************//
struct HoughDataNonP {
    std::vector<cv::Vec2f> lines;
};
ImageUtils::HoughDataNonP houghLinesNonP(cv::Mat& im_src);
cv::Mat drawHoughLinesNonP(ImageUtils::HoughDataNonP hough, cv::Mat im_src);

struct HoughDataP {
    std::vector<cv::Vec4i> lines;
};
ImageUtils::HoughDataP houghLinesP(cv::Mat& im_src);
cv::Mat drawHoughLinesP(ImageUtils::HoughDataP hough, cv::Mat& im_src);

}

#endif // IMAGEUTILS_H
