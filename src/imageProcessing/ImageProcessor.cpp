
#include "ImageProcessor.h"
#include <unistd.h>


// initializer for ImageProcessing class
ImageProcessor::ImageProcessor():
    local_map(70,70),
    /*foundCube(0),
    nearestCubeAngle(0),
    nearestCubeDist(0),
    nearestCubeColor(0)*/
    frame(cv::Scalar(0,0,0))
{

    vid_cap = cv::VideoCapture(0); // need to check what 0 is and is not sketchy

    if(!vid_cap.isOpened()) {
        return;
    }
    vid_cap.set(CV_CAP_PROP_FRAME_WIDTH, 640*FRAME_RESIZE_SCALE);
    vid_cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480*FRAME_RESIZE_SCALE);
    vid_cap.set(CV_CAP_PROP_FPS, 30);
    vid_cap.set(CV_CAP_PROP_EXPOSURE,1);
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

void ImageProcessor::detectWall(cv::Mat& frame) {

    // detect blue line
    WallDetection::detectWall(frame, local_map, ColorDetection::COLOR_LINE_BLUE);

    // detect yellow line

}

void ImageProcessor::detectBlocks(cv::Mat& frame) {

    // cpu time for debug
    BlockDetection::detectBlocks(frame, nearestBlockInfo);
    updateNearestBlockInfoAverage();
}

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

// for other threads to use
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

// refresh local map to zeros
void ImageProcessor::local_map_refresh() {
    local_map.setZeros();
    local_map.setVal(30,30,180);
}

// write image to file for debugging purposes
void ImageProcessor::writeToFile(std::string fn) {
    cv::Mat temp;
    frame.copyTo(temp);
    cv::imwrite(fn,temp);
}

// singled out to avoid adding pointers to each global variable
void ImageProcessor::doStuff() {

    clearCameraCache();

    clock_t start = clock(); // for debug

    vid_cap.retrieve(frame_raw); // get a new frame from camera
    //frame_raw = cv::imread( "images/blocks_1.jpg", CV_LOAD_IMAGE_COLOR ); // bgr

    //cv::resize(frame_raw, frame, cv::Size(0,0), FRAME_RESIZE_SCALE, FRAME_RESIZE_SCALE, cv::INTER_LINEAR);

    //detectWall(frame);
    detectBlocks(frame);


    if(DEBUG == 1) {
        cv::namedWindow("frame",1);
        cv::imshow("frame",frame);
        cv::Mat local_map_im = local_map.cvtImage();
        local_map_refresh();

        cv::namedWindow("www",CV_WINDOW_NORMAL);
        cv::imshow("www",local_map_im);
        cv::waitKey(100);
    }
    // some sort of usleep...

    // for debug
    clock_t end = clock();
    cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;

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

void ImageProcessor::run(ImageProcessor *ImageProcessorPointer) {

    ImageProcessorPointer->clearCameraCache();
    while(ImageProcessorPointer->running) {
        ImageProcessorPointer->doStuff();
        usleep(UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS);
    }

}
