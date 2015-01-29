#include <sstream>
#include <iostream>
#include "ImageProcessor.h"
#include <unistd.h>
#include <signal.h>

ImageProcessor *ImageProcessorPointerForStopping;
int i = 9;


void stopImageProcessor(int signo)
{
    if (signo == SIGINT) {
        std::cout << "SIGINT called" << std::endl;
        ImageProcessorPointerForStopping->running=0;
    }
}

// *** (DE)CONSTRUCTORS FOR IMAGE PROCESSING CLASS *** //
ImageProcessor::ImageProcessor():
    local_map(70,70),
    frame(cv::Scalar(0,0,0))
{
    vid_cap = cv::VideoCapture(0); // need to check what 0 is and is not sketchy

    if(vid_cap.isOpened()) {
        vid_cap.set(CV_CAP_PROP_FRAME_WIDTH, 640*FRAME_RESIZE_SCALE);
        vid_cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480*FRAME_RESIZE_SCALE);
        vid_cap.set(CV_CAP_PROP_FPS, 30);
        vid_cap.set(CV_CAP_PROP_AUTO_EXPOSURE, 0 );
    }
    else if (!DEBUG) {
        return;
    }

    ImageProcessorPointerForStopping = this;
    signal(SIGINT, stopImageProcessor);

    running=1;
    runThread = new std::thread(run,this);

    cpu_time = 0.0; // for debug
    cache_time = 0.0; // for debug
}

ImageProcessor::~ImageProcessor(){
    running=0;
    runThread->join();
    delete runThread;
}

// ************ DO THE STUFF ************* //
void ImageProcessor::detectWall(cv::Mat& frame) {
    local_map_refresh();
    WallDetection::detectWall(frame, local_map);
}

void ImageProcessor::detectBlocks(cv::Mat& frame) {
    BlockDetection::detectBlocks(frame, nearestBlockInfo);
    updateNearestBlockInfoAverage();
}

void ImageProcessor::detectPurpleLine(cv::Mat& frame) {
    TerritoryDetection::detectPurpleLine(frame, local_map);
}

void ImageProcessor::detectPurpleLineTest(cv::Mat& frame) {
    TerritoryDetection::detectPurpleLineTest(frame, local_map);
}

void ImageProcessor::detectPurpleLineTest2(cv::Mat& frame) {
    TerritoryDetection::detectPurpleLineTest2(frame, local_map);
}

// ******* UPDATE INFO ******** //
// average over data to reduce noise/randomness
void ImageProcessor::updateNearestBlockInfoAverage() {

    nearestBlockInfo.found_cube =
            nearestBlockInfoPrevious.found_cube * BLOCK_FOUND_PREVIOUS_WEIGHT
            + nearestBlockInfo.found_cube * (1-BLOCK_FOUND_PREVIOUS_WEIGHT);
    nearestBlockInfo.nearest_cube_angle =
            nearestBlockInfoPrevious.nearest_cube_angle * BLOCK_ANGLE_PREVIOUS_WEIGHT
            + nearestBlockInfo.nearest_cube_angle * (1-BLOCK_ANGLE_PREVIOUS_WEIGHT);
    nearestBlockInfo.nearest_cube_dist =
            nearestBlockInfoPrevious.nearest_cube_dist * BLOCK_DIST_PREVIOUS_WEIGHT
            + nearestBlockInfo.nearest_cube_dist * (1-BLOCK_DIST_PREVIOUS_WEIGHT);
    nearestBlockInfo.nearest_cube_color -
            nearestBlockInfoPrevious.nearest_cube_color * BLOCK_COLOR_PREVIOUS_WEIGHT
            + nearestBlockInfo.nearest_cube_color * (1-BLOCK_COLOR_PREVIOUS_WEIGHT);

    nearestBlockInfoPrevious = nearestBlockInfo;

}

// refresh local map to zeros
void ImageProcessor::local_map_refresh() {
    local_map.setZeros();
    local_map.setVal(30,30,180);
}

// ****** FOR OTHER THREADS TO USE ****** //
int ImageProcessor::getFoundCube() {
    return nearestBlockInfo.found_cube > 0.5;
}
int ImageProcessor::getNearestCubeColor() {
    return nearestBlockInfo.nearest_cube_color > 0.5;
}
double ImageProcessor::getNearestCubeAngle() {
    return nearestBlockInfo.nearest_cube_angle;
}
double ImageProcessor::getNearestCubeDist() {
    return nearestBlockInfo.nearest_cube_dist;
}
// for debug
double ImageProcessor::getCpuTime() {
    return cpu_time;
}
double ImageProcessor::getCacheTime() {
    return cache_time;
}

// write image to file for debugging purposes
void ImageProcessor::writeToFile(std::string fn) {
    cv::Mat temp;
    frame.copyTo(temp);
    cv::imwrite(fn,temp);
}

// ****** MAIN FUNCTION IN LOOP ******* //
//void ImageProcessor::doStuff() {
//    i += 1;
//    std::ostringstream oss;
//    oss << "plines/plines_" << i << ".png";
//    clearCameraCache();
//    clock_t start = clock();

//    vid_cap.retrieve(frame_raw);
//    cv::resize(frame_raw,frame,cv::Size(0,0),1,1,cv::INTER_LINEAR);

//    cv::namedWindow("frame", cv::WINDOW_NORMAL);
//    cv::imshow("frame", frame);

//    std::vector<int> compression_params;
//    compression_params.push_back(16);
//    compression_params.push_back(9);
//    imwrite(oss.str(),frame,compression_params);

//    cv::waitKey(500);

//    clock_t end = clock();
//    cpu_time = ((double) (end-start))/CLOCKS_PER_SEC;
//}

//void ImageProcessor::doStuff() {

//    clearCameraCache();

//    clock_t start = clock(); // for debug

//    vid_cap.retrieve(frame_raw); // get a new frame from camera
//    cv::resize(frame_raw,frame,cv::Size(0,0), 1, 1, cv::INTER_LINEAR);

////    frame = cv::imread("images/blocks_13.jpg");
////    cv::resize(frame,frame,cv::Size(0,0),1,1,cv::INTER_LINEAR);

//    detectWall(frame);
//    local_map_refresh();
//    detectBlocks(frame);
//    detectPurpleLineTest2(frame);

//    cv::namedWindow("frame", cv::WINDOW_NORMAL);
//    cv::imshow("frame", frame);

////    cv::namedWindow("local_map", cv::WINDOW_NORMAL);
////    cv::imshow("local_map", local_map.cvtImage());

//    cv::waitKey(100);

//    // for debug
//    clock_t end = clock();
//    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;

//}

void ImageProcessor::doStuff() {
    i += 1;
    std::ostringstream oss;
    oss << "plines/plines_" << i << ".png";
    clearCameraCache();
    clock_t start = clock();
    if (i <= 84) {
        frame_raw = cv::imread(oss.str(),-1);
        cv::resize(frame_raw,frame,cv::Size(0,0),1,1,cv::INTER_LINEAR);
        //detectWall(frame);
        //local_map_refresh();
        //detectBlocks(frame);
        std::cout << "Image #" << i << ": ";
        detectPurpleLineTest2(frame);

        cv::namedWindow("frame", cv::WINDOW_NORMAL);
        cv::imshow("frame", frame);
    }

    cv::waitKey(333);
    clock_t end = clock();
    cpu_time = ((double) (end-start))/CLOCKS_PER_SEC;
}

void ImageProcessor::clearCameraCache() {
    // for debug
    clock_t start = clock();

    // hack to clean cache from the camera to avoid weird bug in the beginning
    for(int i = 0; i < 4; i++) {
        vid_cap.grab(); // get a new frame from camera
    }

    // for debug
    clock_t end = clock();
    cache_time = ((double) (end - start)) / CLOCKS_PER_SEC;
}

// for debug
void ImageProcessor::debugStuff() {

    //frame_raw = cv::imread("images/test_final/blocks_2.jpg", CV_LOAD_IMAGE_COLOR ); // bgr
    vid_cap.grab(); // get a new frame from camera
    vid_cap.retrieve(frame_raw); // get a new frame from camera
    cv::resize(frame_raw, frame, cv::Size(0,0), 1, 1, cv::INTER_LINEAR);
    clock_t start = clock();
    WallDetection::detectWall(frame, local_map);
    BlockDetection::detectBlocks(frame, nearestBlockInfo);
    TerritoryDetection::detectPurpleLine(frame, local_map);
    clock_t end = clock();
    double dd = ((double) (end - start)) / CLOCKS_PER_SEC;

}

void ImageProcessor::run(ImageProcessor *ImageProcessorPointer) {

    while(ImageProcessorPointer->running) {
        ImageProcessorPointer->doStuff();
        usleep(UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS);
    }
    while(ImageProcessorPointer->running) {
        ImageProcessorPointer->debugStuff();
        usleep(UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS);
    }

}
