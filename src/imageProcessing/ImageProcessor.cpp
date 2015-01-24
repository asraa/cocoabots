
#include "ImageProcessor.h"
#include <unistd.h>


// initializer for ImageProcessing class
ImageProcessor::ImageProcessor():
    local_map(70,70)
    /*foundCube(0),
    nearestCubeAngle(0),
    nearestCubeDist(0),
    nearestCubeColor(0)*/{

    vid_cap = cv::VideoCapture(0); // need to check what 0 is and is not sketchy
    if(!vid_cap.isOpened()) {
        return;
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


}

void ImageProcessor::detectBlocks(cv::Mat& frame) {

    // very crude stuff right now
    BlockDetection::detectBlocks(frame, nearestBlockInfo);
    updateNearestBlockInfoAverage();
}

void ImageProcessor::updateNearestBlockInfoAverage() {

    BlockDetection::BlockInfo tempBlock = nearestBlockInfo;

    nearestBlockInfo.found_cube =
            nearestBlockInfoPrevious.found_cube * BLOCK_FOUND_PREVIOUS_WEIGHT
            + nearestBlockInfoPrevious.found_cube * (1-BLOCK_FOUND_PREVIOUS_WEIGHT);
    nearestBlockInfo.nearest_cube_angle =
            nearestBlockInfoPrevious.nearest_cube_angle * BLOCK_ANGLE_PREVIOUS_WEIGHT
            + nearestBlockInfo.nearest_cube_angle * (1-BLOCK_ANGLE_PREVIOUS_WEIGHT);
    nearestBlockInfo.nearest_cube_dist =
            nearestBlockInfoPrevious.nearest_cube_dist * BLOCK_DIST_PREVIOUS_WEIGHT
            + nearestBlockInfo.nearest_cube_dist * (1-BLOCK_DIST_PREVIOUS_WEIGHT);
    nearestBlockInfo.nearest_cube_color -
            nearestBlockInfoPrevious.nearest_cube_color * BLOCK_COLOR_PREVIOUS_WEIGHT
            + nearestBlockInfo.nearest_cube_color * (1-BLOCK_COLOR_PREVIOUS_WEIGHT);

    nearestBlockInfoPrevious = tempBlock;

}

int ImageProcessor::getFoundCube() {
    return nearestBlockInfo.found_cube;
}

int ImageProcessor::getNearestCubeColor() {
    return nearestBlockInfo.nearest_cube_color;
}

double ImageProcessor::getNearestCubeAngle() {
    return nearestBlockInfo.nearest_cube_angle;
}

double ImageProcessor::getNearestCubeDist() {
    return nearestBlockInfo.nearest_cube_dist;
}

void ImageProcessor::local_map_refresh() {
    local_map.setZeros();
    local_map.setVal(30,30,180);
}

void ImageProcessor::run(ImageProcessor *ImageProcessorPointer) {

    cv::Mat frame_raw;
    cv::Mat frame;

    // hack to clean cache from the camera to avoid weird bug in the beginning
    for(int i = 0; i < 10; i++) {
        ImageProcessorPointer->vid_cap >> frame_raw; // get a new frame from camera
        usleep(UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS);
    }

    while(ImageProcessorPointer->running) {

        /*Eigen::Vector2d pt_im;
        pt_im << 163,124;
        std::cout<<"result" << CameraMath::reconstructPoint2D(pt_im, 2)<<std::endl;
        Eigen::Vector2d result = CameraMath::reconstructPoint2D(pt_im, 2);
        std::cout<< "radial" << CameraMath::cvtCamXY2RobotRadial(result[0],result[1])<<std::endl;
        std::cout<< CAM_MAT<<std::endl;
        std::cout<<ROT_MAT<<std::endl;
        std::cout<<CAM_MAT_INV<<std::endl;*/

        ImageProcessorPointer->vid_cap >> frame_raw; // get a new frame from camera
        //frame_raw = cv::imread( "images/blocks_1.jpg", CV_LOAD_IMAGE_COLOR ); // bgr

        cv::resize(frame_raw, frame, cv::Size(0,0), FRAME_RESIZE_SCALE, FRAME_RESIZE_SCALE, cv::INTER_LINEAR);

        //ImageProcessorPointer->detectWall(frame);
        ImageProcessorPointer->detectBlocks(frame);


        if(DEBUG == 1) {
            cv::namedWindow("frame",1);
            cv::imshow("frame",frame);
            cv::Mat local_map_im = ImageProcessorPointer->local_map.cvtImage();
            ImageProcessorPointer->local_map_refresh();

            cv::namedWindow("www",CV_WINDOW_NORMAL);
            cv::imshow("www",local_map_im);
            cv::waitKey(100);
        }
        // some sort of usleep...
        usleep(UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS);
    }

}
