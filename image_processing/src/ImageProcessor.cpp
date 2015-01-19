
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

    // do i need contour class??
    BlockDetection::detectBlock(frame);


    // get contour
    std::cout<<"hello";
    // for each contour:
    // determine distance
    //

}



void ImageProcessor::run() {
    cv::namedWindow("video",1); // what's the 1 for?
    cv::namedWindow("local map",cv::WINDOW_NORMAL);
    cv::namedWindow("raw", 1);
  //  while(1) {
        local_map.setZeros();
        local_map.setVal(30,30,180);
        cv::Mat frame;
        //vid_cap >> frame; // get a new frame from camera
        //cv::imshow("raw",frame);

        frame = cv::imread( "images/calibration_2.jpg", CV_LOAD_IMAGE_COLOR ); // bgr

        // test
        detectWall(frame);
        //detectBlocks(frame);

        cv::Mat local_map_im = local_map.cvtImage();

        cv::imshow("video", frame);
        cv::imshow("local map", local_map_im);
         cv::waitKey(0); // somehow must need this to work
   //}
}

/*
 Ideally what i want for these functions:

 detectwall(){
    what's involved in here?
    - get blue color -- ColorDetection
    - canny edges/contour
    - check if is wall (if using contour)
    - search line by line -- function that does this?? sure.
    - remove pixels above -- in the function above
    - determine x,y coords in camera coord
    -- do the same for yellow


    -- do the same for red (<-- could be problematic???)

    - get blue & yellow & white -- colordetection
    - hough lines
    - determine x,y coords using line info -- function
 }

 detectblocks(){

 }

 run() {

 while(1) {
 detectwall();
 detectblocks();
 update();
 }

 }




 * */

