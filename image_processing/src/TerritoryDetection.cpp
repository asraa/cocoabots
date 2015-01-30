
#include "TerritoryDetection.h"

// things this needs to to do

// determine purple line

namespace TerritoryDetection {

void addHoughLine(cv::Vec2f& line, cv::Mat& out, CvScalar color, int width, int factor) {
    // inserts a single Hough line into out
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

void findEndpoints(cv::Vec2f& line, cv::Mat& out) {
    // !! NEEDS IMPLEMENTATION !! //
    return 0;
}



void addHoughLines(std::vector<cv::Vec2f>& lines, cv::Mat& out, CvScalar color, int width, int factor) {
    // adds multiple hough lines
    for(size_t i = 0; i < lines.size(); i++) {
        addHoughLine(lines[i],out,color,width,factor);
    }
}

cv::Vec4i getVectorFromHoughLine(cv::Vec2f& line, int factor) {
    // self explanatory
    float rho = line[0], theta = line[1];
    cv::Point pt1, pt2;
    double a = cos(theta), b = sin(theta);
    double x0 = a*rho, y0 = b*rho;
    pt1.x = round(x0 + factor*(-b));
    pt1.y = round(y0 + factor*(a));
    pt2.x = round(x0 - factor*(-b));
    pt2.y = round(y0 - factor*(a));
    cv::Vec4i vec = (pt1.x, pt1.y, pt2.x, pt2.y);
    return vec;
}

int getAngleBetweenHoughLines(cv::Vec2f v1, cv::Vec2f v2) {
    // signifcantly more reliable than angle between vectors
    int result = 180*fabs(v2[1]-v1[1])/CV_PI;
    return result;
}

std::vector<cv::Vec2f> removeDuplicates(std::vector<cv::Vec2f> lines, double threshold=5) {
    // removes duplicate hough lines, or those that have similar angles
    // specified in degrees. pretty useful if you can't get good threshold settings.
    std::vector<cv::Vec2f> tempVec;
    bool unique;
    for (int i = 0; i < lines.size(); i++) {
        unique = true;
        for (int j = 0; j < tempVec.size(); j++) {
            if (getAngleBetweenHoughLines(lines[i],tempVec[j]) < threshold) {
                unique = false;
            }
        }
        if (unique) {
            tempVec.push_back(lines[i]);
        }
    }
    return tempVec;
}

void mainProcess(cv::Mat& src) {
    int width = 10, factor = 1000, threshold = 60;

    cv::Mat dst, cdst;
    std::vector<cv::Vec2f> lines;

    cv::Mat purp = ColorDetection::detectColor(src,ColorDetection::COLOR_LINE_PURPLE);

    cv::Canny(purp,dst,50,200,3);
    cv::cvtColor(dst,cdst,CV_GRAY2BGR);

    cv::HoughLines(dst,lines,1,1*CV_PI/180,threshold,0,0);

    lines = removeDuplicates(lines,30.0);

    addHoughLines(lines,src,cv::Scalar(0,0,255),width,factor);

    cv::Mat output;

    cv::Mat lines_only(purp.rows,purp.cols,CV_8UC1);

    std::cout << "there are " << lines.size() << " lines" << std::endl;

    for (int i = 0; i < lines.size(); i++) {
        addHoughLine(lines[i],lines_only,cv::Scalar(255,255,255),width,factor);
        cv::bitwise_and(purp,lines_only,output);
//        cv::RotatedRect rot = cv::minAreaRect(output);
//        cv::Point2f vtx[4];
//        rot.points(vtx);
//        std::cout << "vtx: " << vtx[0] << " " << vtx[1] << " " << vtx[2] << " " << vtx[3] << std::endl;
        displayMat(src,"src!");
        displayMat(output,"output!");

        cv::waitKey(5000);
        output = cv::Mat::zeros(purp.rows,purp.cols,CV_8UC1);
        lines_only = cv::Mat::zeros(purp.rows,purp.cols,CV_8UC1);
    }
}

void goThroughPixels(cv::Mat& im_purp_contour, GridMap& local_map) {
    int num_rows = im_purp_contour.rows;
    int num_cols = im_purp_contour.cols;//FRAME_SIZE_X;
    for (int i=0; i<num_rows; i++) { // from bottom to top
        for (int j=0; j<num_cols; j++) {
            //std::cout<<i<<" "<<j;
            uchar tmp = im_purp_contour.at<uchar>(i,j); // get gray value
            if(tmp == COLOR_WHITE) {
                Eigen::Vector2d pt_im;
                pt_im << j, i; // col, row --> x, y
                Eigen::Vector2d pt_c = CameraMath::reconstructPoint2D(pt_im, 0); // on the floor
                int x = floor(30+pt_c[0]); // hacking for now
                int y = floor(30+pt_c[1]);
                local_map.setVal(x,y,255);
            }
        }
    }
}


// should i first do find contour to find area of feature?
void detectPurpleLine(cv::Mat& frame, GridMap& local_map) {

    // get purple line
    cv::Mat im_purp = ColorDetection::detectColor(frame, ColorDetection::COLOR_LINE_PURPLE);
    // do a canny edge detection
    cv::Mat purp_canny = ImageUtils::cannyEdge(im_purp);

    ImageUtils::ContourData purp_contour_data = ImageUtils::getContours(im_purp);
    ImageUtils::cleanContour(purp_contour_data, FEATURE_AREA_THRESH_PURP_LINE);
    cv::Mat purp_contour = ImageUtils::drawContoursFilled(purp_contour_data, im_purp);

    goThroughPixels(purp_contour, local_map);

    // do a hough line fit

    // check line properties


}

// x0,y0 is origin; x1,y1 and x2,y2 are line segment endpoints
double distToLineSeg(double x0, double y0, double x1, double y1, double x2, double y2) {
    double dist;


    return dist;
}

std::vector findDistances(std::vector endpoints, double x0, double y0) {

    std::vector result;

    for(int i = 0; i < endpoints.size(); i = i+4) {
        double x1 = endpoints.at(i);
        double y1 = endpoints.at(i+1);
        double x2 = endpoints.at(i+2);
        double y2 = endpoints.at(i+3);
        double dist = distToLineSeg(x0,y0,x1,y1,x2,y2);
        result.push_back(dist);
    }

}

void determinePurpleLineInfo() {

     std::vector endpoints;

     std::vector line_seg_dist = findDistances(endpoints);

     // first sort the distances
     int min_dist_ind = findMinDistInd(line_seg_dist);

     // find point on line


     // find dist and angle





}

    // args= image, lines, rho, theta, threshold, minlinelength, maxlinegap

    // do a hough line fit

    // check line properties


}
