#include "ContourUtils.h"

namespace ContourUtils {

ContourData getContours(cv::Mat& im_src) {
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Mat im_src_copy;
    im_src.copyTo( im_src_copy);
    cv::findContours(im_src_copy, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    ContourData contour_data;
    contour_data.contours = contours;
    contour_data.hierarchy = hierarchy;

    return contour_data;

}

bool contour2small(std::vector<cv::Point>& contour) {
    return (fabs(cv::contourArea(contour,0)) < FEATURE_AREA_THRESH);
}

// VERY INEFFICIENT
void cleanContour(ContourData& contour_data) {
    std::vector<int> to_be_removed;
    for(int i = 0; i < contour_data.contours.size(); i++) {
        if(contour2small(contour_data.contours.at(i))){
            to_be_removed.push_back(i);
        }
    }
    for(int i = to_be_removed.size()-1; i > - 1; i--) {
        contour_data.contours.erase(contour_data.contours.begin()+i);
        contour_data.hierarchy.erase(contour_data.hierarchy.begin()+i);
    }
}


cv::Mat drawContours(ContourData& contour_data, cv::Mat& frame) {
    cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);
    std::cout<<"inside" <<contour_data.hierarchy.size();
    cv::drawContours(drawing, contour_data.contours, -1, cv::Scalar(255,255,255), 3, 8);
    cv::cvtColor(drawing,drawing,CV_BGR2GRAY);
    return drawing;
}


}
