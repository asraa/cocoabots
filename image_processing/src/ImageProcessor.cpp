
#include "ImageProcessor.h"
#include <unistd.h>


// initializer for ImageProcessing class
ImageProcessor::ImageProcessor():
    local_map(70,70),
    foundCube(0),
    nearestCubeAngle(0),
    nearestCubeDist(0),
    nearestCubeColor(0){

    vid_cap = cv::VideoCapture(0); // need to check what 0 is and is not sketchy
    if(!vid_cap.isOpened()) {
        // do something ??
    }
    running=1;
    runThread = new std::thread(run,this);

}

ImageProcessor::~ImageProcessor(){
    running=0;
    runThread->join();
    delete runThread;
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
    BlockDetection::detectBlock(frame, foundCube, nearestCubeAngle, nearestCubeDist, nearestCubeColor);

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

void ImageProcessor::local_map_refresh() {
    local_map.setZeros();
    local_map.setVal(30,30,180);
}

void ImageProcessor::run(ImageProcessor *ImageProcessorPointer) {
    while(ImageProcessorPointer->running) {

        cv::Mat frame_raw;
        ImageProcessorPointer->vid_cap >> frame_raw; // get a new frame from camera

        cv::Mat frame;
        // hard-coding resize_ratio for now
        cv::resize(frame_raw, frame, cv::Size(0,0), 0.5, 0.5, cv::INTER_LINEAR);
        //frame = cv::imread( "images/calibration_19.jpg", CV_LOAD_IMAGE_COLOR ); // bgr

        // test
        ImageProcessorPointer->detectWall(frame);
        ImageProcessorPointer->detectBlocks(frame);

        if(DEBUG == 1) {
            //  cv::Mat local_map_im = local_map.cvtImage();
            //cv::namedWindow("www",1);
            // cv::imshow("www",local_map_im);
            cv::waitKey(100);
        }

        // some sort of usleep...
        usleep(UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS);
    }

}
