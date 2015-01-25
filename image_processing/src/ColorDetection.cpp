
#include "ColorDetection.h"

namespace ColorDetection {

// dectect colors
cv::Mat detectColor(cv::Mat& frame, int color){

    // hacking hsv values for now

    cv::Mat frame_hsv;
    cv::cvtColor(frame, frame_hsv, CV_BGR2HSV);

    cv::Mat result;

    if(color == COLOR_LINE_BLUE) {
        inRange(frame_hsv, cv::Scalar(100,100,0), cv::Scalar(130,255,255), result);
    } else if (color == COLOR_BLOCK_GREEN) {
        inRange(frame_hsv, cv::Scalar(50,100,0), cv::Scalar(100,255,255), result);
    } else if (color == COLOR_BLOCK_RED) {
        inRange(frame_hsv, cv::Scalar(0,100,0), cv::Scalar(50,255,255), result);
        cv::Mat result2;
        inRange(frame_hsv, cv::Scalar(150,100,0), cv::Scalar(255,255,255), result2);
        result = result + result2;
    } else if (color == COLOR_LINE_PURPLE) {
        inRange(frame_hsv, cv::Scalar(120,30,0), cv::Scalar(155,255,255), result);
    }
    return result;
}


bool isRedBlock() {

}


// takes in colors array to account or different combinations of colors
// e.g. blue & white --> wall / red & green --> stacks of blocks
cv::Mat detectColor(cv::Mat& frame, int colors[], int size_of_colors){

    cv::Mat result;

    // temp color detection
    for(int i = 0; i < size_of_colors; i++) {
        cv::Mat temp = detectColor(frame, colors[i]);
        result = cv::max(result, temp);
    }
    return result;

}






}
