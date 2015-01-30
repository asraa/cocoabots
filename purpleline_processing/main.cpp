#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

int main(int argc, char** argv) {
    int i = 0;
    while (i <= 84) {
        i += 1;
        std::ostringstream oss;
        oss << "plines/plines_" << i << ".png";
        cv::Mat src = cv::imread(oss.str(),-1);
        cv::Mat dst, cdst;
        cv::Canny(src,dst,50,200,3);
        cv::cvtColor(dst,cdst, CV_GRAY2BGR);
        std::vector<cv::Vec2f> lines;
        cv::HoughLines(dst,lines,1,CV_PI/180,150,0,0);
        for(size_t k = 0; k < lines.size(); k++) {
            float rho = lines[k][0], theta = lines[k][1];
            cv::Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = round(x0 + 1000*(-b));
            pt1.y = round(y0 + 1000*(a));
            pt2.x = round(x0 - 1000*(-b));
            pt2.y = round(y0 - 1000*(a));
            cv::line(cdst,pt1,pt2,cv::Scalar(0,0,255),3,CV_AA);
        }
        cv::namedWindow("source",cv::WINDOW_NORMAL);
        cv::namedWindow("detected lines", cv::WINDOW_NORMAL);
        cv::imshow("source", src);
        cv::imshow("detected lines", cdst);
        cv::waitKey(500);
    }
    return 0;
}
