
#include "ImageProcessor.h"


// initializer for ImageProcessing class
ImageProcessor::ImageProcessor(): local_map(70,70) {

    vid_cap = cv::VideoCapture(0); // need to check what 0 is and is not sketchy
    if(!vid_cap.isOpened()) {
        // do something ??
    }

}

void ImageProcessor::detectWall(cv::Mat& frame) {

    // detect blue line
    WallDetection::detectWall(frame, local_map, ColorDetection::COLOR_LINE_BLUE);

    // detect yellow line

    // detect red line (somehow)

}

void ImageProcessor::detectBlocks(cv::Mat& frame) {

    // first find both colors??

    // get red + green

    // very crude stuff right now
    BlockDetection::detectBlock(frame, foundCube, nearestCubeAngle, nearestCubeDist);

}

int ImageProcessor::getFoundCube() {
    return foundCube;
}

int ImageProcessor::getNearestCubeColor() {
    return nearestCubeColor;
}

double ImageProcessor::getNearestCubeAngle() {
    return nearestCubeAngle;
}

double ImageProcessor::getNearestCubeDist() {
    return nearestCubeDist;
}

void ImageProcessor::run() {
 while(1) {
        local_map.setZeros();
        local_map.setVal(30,30,180);
        cv::Mat frame;
        vid_cap >> frame; // get a new frame from camera

        //frame = cv::imread( "images/calibration_19.jpg", CV_LOAD_IMAGE_COLOR ); // bgr

        // test
        detectWall(frame);
        detectBlocks(frame);

      //  cv::Mat local_map_im = local_map.cvtImage();
        //cv::namedWindow("www",1);
       // cv::imshow("www",local_map_im);
        cv::waitKey(100);
 }
}
