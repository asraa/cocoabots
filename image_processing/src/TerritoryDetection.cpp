
#include "TerritoryDetection.h"

// things this needs to to do

// determine purple line
// see if it's black black or black white
// ie. if it splits the field or is up against a wall

namespace TerritoryDetection {

bool isUpAgainstWall() {

}


// should i first do find contour to find area of feature?
void detectPurpleLine(cv::Mat& frame) {

    // get purple line
    cv::Mat im_purp = ColorDetection::detectColor(frame, ColorDetection::COLOR_LINE_PURPLE);
    // do a canny edge detection
    cv::Mat purp_canny = ImageUtils::cannyEdge(im_purp);

    cv::Mat purp_contour = ImageUtils::getContoursMat(im_purp);

    cv::namedWindow("po",1);
    cv::imshow("po",purp_contour);

    ImageUtils::HoughDataNonP purp_hough = ImageUtils::houghLinesNonP(purp_canny);
    cv::namedWindow("aa",1);
    cv::imshow("aa",ImageUtils::drawHoughLinesNonP(purp_hough, frame));

    ImageUtils::HoughDataP purp_hough_p = ImageUtils::houghLinesP(purp_canny);
    cv::imshow("bb",ImageUtils::drawHoughLinesP(purp_hough_p, frame));

    ImageUtils::HoughDataP purp_hough_p_c = ImageUtils::houghLinesP(purp_contour);
    cv::imshow("qq",ImageUtils::drawHoughLinesP(purp_hough_p_c, frame));

    cv::waitKey(100);
    // do a hough line fit

    // check line properties


}

}
