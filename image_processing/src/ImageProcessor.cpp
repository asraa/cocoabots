
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

    // very crude stuff right now
    BlockDetection::detectBlocks(frame, nearestBlockInfo);

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

    while(ImageProcessorPointer->running) {

        /*Eigen::Vector2d pt_im;
        pt_im << 163,124;
        std::cout<<"result" << CameraMath::reconstructPoint2D(pt_im, 2)<<std::endl;
        Eigen::Vector2d result = CameraMath::reconstructPoint2D(pt_im, 2);
        std::cout<< "radial" << CameraMath::cvtCamXY2RobotRadial(result[0],result[1])<<std::endl;
        std::cout<< CAM_MAT<<std::endl;
        std::cout<<ROT_MAT<<std::endl;
        std::cout<<CAM_MAT_INV<<std::endl;*/

        cv::Mat frame_raw;
        ImageProcessorPointer->vid_cap >> frame_raw; // get a new frame from camera

        cv::Mat frame;
        // hard-coding resize_ratio for now
        //cv::resize(frame_raw, frame, cv::Size(0,0), 0.5, 0.5, cv::INTER_LINEAR);
        frame = cv::imread( "images/calibration_19.jpg", CV_LOAD_IMAGE_COLOR ); // bgr
        cv::resize(frame, frame, cv::Size(0,0), FRAME_RESIZE_SCALE, FRAME_RESIZE_SCALE, cv::INTER_LINEAR);

        //frame = frame_raw;

        // test
        ImageProcessorPointer->detectWall(frame);
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
