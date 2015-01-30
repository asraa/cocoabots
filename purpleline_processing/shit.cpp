#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include "TerritoryDetection.h"
#include "ColorDetection.h"
#include "ImageUtils.h"
#include "CameraConfig.h"

void addHoughLine(cv::Vec2f& line, cv::Mat& out, CvScalar color, int width, int factor) {
    float rho = line[0], theta = line[1];
    cv::Point pt1, pt2;
    double a = cos(theta), b = sin(theta);
    double x0 = a*rho, y0 = b*rho;
    pt1.x = round(x0 + factor*(-b));
    pt1.y = round(y0 + factor*(a));
    pt2.x = round(x0 - factor*(-b));
    pt2.y = round(y0 - factor*(a));
    cv::line(out,pt1,pt2,color,width,CV_AA);
}

void addHoughLines(std::vector<cv::Vec2f>& lines, cv::Mat& out, CvScalar color, int width, int factor) {
    for(size_t i = 0; i < lines.size(); i++) {
        addHoughLine(lines[i],out,color,width,factor);
    }
}

void displayMat(cv::Mat& src, std::string title) {
    cv::namedWindow(title,cv::WINDOW_NORMAL);
    cv::imshow(title,src);
}

void processFileImage(std::string filename) {
    cv::Mat src, dst, cdst, frame;
    std::vector<cv::Vec2f> lines;

    src = cv::imread(filename,1);
    src.copyTo(frame);
    cv::Canny(src,dst,50,200,3);
        // Canny args: inp image, out edgemap, double thresh1, double thresh2, int aperturesize
    cv::cvtColor(dst,cdst,CV_GRAY2BGR);
    cv::HoughLines(dst,lines,1,CV_PI/180,150,0,0);
        // HoughLines args: inp image, out lines, double rho, double theta, int threshold
    addHoughLines(lines,frame,cv::Scalar(0,0,255),3,1000);
    displayMat(frame,"frame");
    cv::waitKey(1000);
}

void processFileLocalMap(std::string filename, int size) {
    cv::Mat src, dst, cdst, frame, local_map_mat;
    std::vector<cv::Vec2f> lines;
    GridMap local_map(size,size);

    src = cv::imread(filename,1);
    src.copyTo(frame);
    TerritoryDetection::detectPurpleLine(frame,local_map);
    local_map_mat = local_map.cvtImage();
    cv::HoughLines(local_map_mat,lines,4,4*CV_PI/180,25,0,0);
    cv::cvtColor(local_map_mat,local_map_mat,CV_GRAY2BGR);
    addHoughLines(lines,local_map_mat,cv::Scalar(0,0,255),1,50);
    std::cout << filename << ": number of lines = " << lines.size() << std::endl;
    displayMat(local_map_mat,"local_map");
    displayMat(src,"source");
    cv::waitKey(1000);
}

cv::Mat getPurpContour(cv::Mat& src) {
    cv::Mat im_purp = ColorDetection::detectColor(src,ColorDetection::COLOR_LINE_PURPLE);
    ImageUtils::ContourData purp_contour_data = ImageUtils::getContours(im_purp);
    ImageUtils::cleanContour(purp_contour_data, FEATURE_AREA_THRESH_PURP_LINE);
    cv::Mat purp_contour = ImageUtils::drawContoursFilled(purp_contour_data,im_purp);
    return purp_contour;
}

void processFile(std::string filename) {
    cv::Mat src, src_canny, contour_lines_mat;
    std::vector<cv::Vec2f> contour_lines, canny_lines;

    src = cv::imread(filename,1);
    src.copyTo(src_canny);


    // Generate empty matrix featuring canny lines of extra width
    cv::Canny(src,src_canny,50,200,3);
//    ImageUtils::cannyEdge(src_canny);
    displayMat(src_canny,"src_canny");
    src_canny.copyTo(contour_lines_mat);
    contour_lines_mat.setTo(cv::Scalar(0,0,0));
    cv::HoughLines(src_canny,canny_lines,1,CV_PI/180,150,0,0);
    addHoughLines(canny_lines,contour_lines_mat,cv::Scalar(255,255,255),10,1000);
    displayMat(contour_lines_mat,"contour_lines_mat");

    // Generate purple contour array
    cv::Mat purp_contour = getPurpContour(src);
    cv::cvtColor(purp_contour,purp_contour,CV_GRAY2BGR);

    std::cout << "size of canny_lines " << canny_lines.size() << std::endl;
//     Binary AND operation between purp_contour and contour_lines_mat
    cv::Mat purp_contour_binand;
    purp_contour.copyTo(purp_contour_binand,contour_lines_mat);
    displayMat(purp_contour_binand,"after binand operation");
    cv::waitKey(5000);
}

void processFile2(std::string filename) {
    int threshold = 250;
    int width = 10;
    int factor = 100;

    cv::Mat src, contour_lines_mat;
    std::vector<cv::Vec2f> contour_lines;
    src = cv::imread(filename,1);

    //make empty mat, best way?
    src.copyTo(contour_lines_mat);
    contour_lines_mat.setTo(cv::Scalar(0,0,0));

    cv::Mat purp_contour = getPurpContour(src);
    cv::HoughLines(purp_contour,contour_lines,1,5*CV_PI/180,threshold,0,0);

    std::cout << "Found " << contour_lines.size() << " purp_contour lines" << std::endl;

    addHoughLines(contour_lines,contour_lines_mat,cv::Scalar(255,255,255),width,factor);

    displayMat(contour_lines_mat,"contour_lines_mat");
    displayMat(purp_contour,"purp_contour");
    cv::waitKey(5000);
}

void processFile3(std::string filename) {
    int threshold = 150;
    int width = 5;
    int factor = 10;

    cv::Mat src, src_canny, canny_lines_mat, tempMat;
    std::vector<cv::Vec2f> canny_lines;
    src = cv::imread(filename,1);

    //make empty mat, best way?
    src.copyTo(canny_lines_mat);
    src.copyTo(tempMat);
    canny_lines_mat.setTo(cv::Scalar(0,0,0));
    tempMat.setTo(cv::Scalar(0,0,0));

    cv::Mat purp_contour = ColorDetection::detectColor(src, ColorDetection::COLOR_LINE_PURPLE);


    cv::Canny(src,src_canny,50,200,3);
    cv::HoughLines(purp_contour,canny_lines,1,CV_PI/180,threshold,0,0);
    //cv::Mat purp_contour = getPurpContour(src);
    cv::Mat all_canny_lines_mat;
    addHoughLines(canny_lines,src,cv::Scalar(0,0,255),width,factor);

    std::cout << "Found " << canny_lines.size() << "lines." << std::endl;

    displayMat(src, "linez");

    displayMat(purp_contour,"purp");

//    for (int i = 0; i < canny_lines.size(); i++) {
//        std::cout << "line " << i << "--" << std::endl;
//        std::cout << "pt1: (" << canny_lines[i][0] << "," << canny_lines[i][1] << std::endl;
//    }


//    if (canny_lines.size() == 0) {
//        std::cout << "fuck there aint any contours, sorry bud" << std::endl;
//    }
//    else {
//        cv::Mat maxMatContour, maxMatLine;
//        double maxVal = 0;
//        int maxLine = 0;
//        for (int i = 0; i < canny_lines.size(); i++) {
//            addHoughLine(canny_lines[i],canny_lines_mat,cv::Scalar(255,255,255),width,factor);
//            std::cout << "purp_countor size: " << purp_contour.rows << " by " << purp_contour.cols << std::endl;
//            std::cout << "canny_lines_mat size: " << canny_lines_mat.rows << " by " << canny_lines_mat.cols << std::endl;
//            purp_contour.convertTo(purp_contour,CV_8U);
//            canny_lines_mat.convertTo(canny_lines_mat,CV_8U);
//            tempMat.convertTo(tempMat,CV_8U);
////            purp_contour.copyTo(tempMat,canny_lines_mat);
//            cv::bitwise_and(purp_contour,canny_lines_mat,tempMat);
//            double tempVal = cv::sum(tempMat)[0];
//            if (tempVal > maxVal) {
//                tempMat.copyTo(maxMatContour);
//                canny_lines_mat.copyTo(maxMatLine);
//                maxVal = tempVal;
//                maxLine = i;
//            }
//            tempMat.setTo(cv::Scalar(0,0,0));
//            canny_lines_mat.setTo(cv::Scalar(0,0,0));
//        }
//        std::cout << "The max line has area " << maxVal << std::endl;
//        displayMat(purp_contour,"purp_contours");
//        displayMat(all_canny_lines_mat, "all canny lines");
//        displayMat(maxMatContour,"max area contour");
//        displayMat(maxMatLine,"max area canny line");
//    }

    cv::waitKey(100000);
}



int main_nope(int argc, char** argv) {
    int i = 1;
    while (i <= 84) {
        i += 1;
        std::ostringstream oss;
        oss << "plines/plines_" << i << ".png";
        processFile(oss.str());
    }
    return 0;
}
