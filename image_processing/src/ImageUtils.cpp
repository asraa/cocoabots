
#include "ImageUtils.h"


namespace ImageUtils{

// ********* PREPROCESS ********** //
void binaryImagePreProcess(cv::Mat& im_src, int morph_op){
    // smooth the image
    cv::blur(im_src, im_src, cv::Size(SMOOTH_KERNEL,SMOOTH_KERNEL));
    // remove random bits
    cv::Mat clean_kernel = cv::Mat::ones(MORPH_KERNEL, MORPH_KERNEL, CV_8U);
    cv::morphologyEx(im_src, im_src, morph_op, clean_kernel);
}

// ******** CANNY EDGE ********** //
cv::Mat cannyEdge(cv::Mat& im_src){
    cv::Mat im_result;
    cv::Canny(im_src, im_result, CANNY_THRESH_LOW, CANNY_THRESH_UP, CANNY_KERNEL); // returns gray
    cv::dilate(im_result, im_result, cv::Mat(), cv::Point(-1,-1));// added this 01.17
    return im_result;
}

// ******* CONTOUR STUFF ********** //
ContourData getContours(cv::Mat& im_src) {
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Mat im_src_copy;
    im_src.copyTo( im_src_copy);
    cv::findContours(im_src_copy, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    ContourData contour_data;
    contour_data.contours = contours;
    //contour_data.hierarchy = hierarchy;

    return contour_data;
}

ContourData getContourPolyMat(cv::Mat& im_src) {
    ContourData contour_data = getContours(im_src);
    
}

cv::Mat drawContours(ContourData& contour_data, cv::Mat& im_src) {
    cv::Mat drawing = cv::Mat::zeros(im_src.size(), CV_8UC3);
    cv::drawContours(drawing, contour_data.contours, -1, cv::Scalar(255,255,255));
    cv::cvtColor(drawing,drawing,CV_BGR2GRAY);
    return drawing;
}

cv::Mat drawContoursFilled(ContourData& contour_data, cv::Mat& im_src) {
    cv::Mat drawing = cv::Mat::zeros(im_src.size(), CV_8UC3);
    cv::drawContours(drawing, contour_data.contours, -1, cv::Scalar(255,255,255), -1);
    cv::cvtColor(drawing,drawing,CV_BGR2GRAY);
    return drawing;
}

cv::Mat getContoursMat(cv::Mat& im_src) {
    ContourData contour_data = getContours(im_src);
    return drawContours(contour_data, im_src);
}

// VERY INEFFICIENT
void cleanContour(ContourData& contour_data, double area) {
    std::vector<int> to_be_removed;
    for(int i = 0; i < contour_data.contours.size(); i++) {
        if(fabs(cv::contourArea(contour_data.contours.at(i),0)) < area) {
            to_be_removed.push_back(i);
        }
    }
    int j;
    for(int i = to_be_removed.size()-1; i > - 1; i--) {
        j = to_be_removed.at(i);
        contour_data.contours.erase(contour_data.contours.begin()+j);
//        contour_data.hierarchy.erase(contour_data.hierarchy.begin()+i); // probably a bad idea
    }
}

void replaceByPolyContours(ImageUtils::ContourData& contour_data) {
    for(int i = 0; i < contour_data.contours.size(); i++) {
        std::vector<cv::Point> contour = contour_data.contours.at(i);
        cv::approxPolyDP(cv::Mat(contour),contour_data.contours.at(i), POLY_NEIGHBORHOOD, true);
    }
}


// ********** HOUGH LINE STUFF ************//
ImageUtils::HoughDataNonP houghLinesNonP(cv::Mat& im_src) {
    cv::Mat im_copy;
    im_src.copyTo(im_copy);
    ImageUtils::HoughDataNonP hough;
    HoughLines(im_src, hough.lines,1,5*3.14/180.0,100,0,0);
    //HOUGH_RES_RHO_PIXEL, HOUGH_RES_THETA_RAD,
    //HOUGH_MIN_VOTES, 0, 0);
    return hough;
}

cv::Mat drawHoughLinesNonP(ImageUtils::HoughDataNonP hough, cv::Mat im_src) {
    cv::Mat im_copy;
    im_src.copyTo(im_copy);
    for( int i = 0; i < hough.lines.size(); i++ ) {
        double rho = hough.lines[i][0], theta = hough.lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 200*(-b));
        pt1.y = cvRound(y0 + 200*(a));
        pt2.x = cvRound(x0 - 200*(-b));
        pt2.y = cvRound(y0 - 200*(a));
        line(im_copy, pt1, pt2, cv::Scalar(0,0,255), 2, CV_AA);
    }
    return im_copy;
}

ImageUtils::HoughDataP houghLinesP(cv::Mat& im_src) {
    cv::Mat im_copy;
    im_src.copyTo(im_copy);
    ImageUtils::HoughDataP hough;
    HoughLinesP(im_copy, hough.lines,
                HOUGH_P_RES_RHO_PIXEL, HOUGH_P_RES_THETA_RAD,
                HOUGH_P_MIN_VOTES, HOUGH_P_MAX_LINE_GAP);
    return hough;
}


cv::Mat drawHoughLinesP(ImageUtils::HoughDataP hough, cv::Mat& im_src) {
    cv::Mat im_copy;
    im_src.copyTo(im_copy);
    for(int i = 0; i < hough.lines.size(); i++ ){
        cv::Vec4i l = hough.lines[i];
        line(im_copy, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 1, CV_AA);
    }
    return im_copy;
}


}
