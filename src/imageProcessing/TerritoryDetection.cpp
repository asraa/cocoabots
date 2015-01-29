
#include "TerritoryDetection.h"

// things this needs to to do

// determine purple line

namespace TerritoryDetection {

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


    // args= image, lines, rho, theta, threshold, minlinelength, maxlinegap

    // do a hough line fit

    // check line properties


}
