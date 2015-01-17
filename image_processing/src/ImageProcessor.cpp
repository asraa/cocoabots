
#include "ImageProcessor.h"


// initializer for ImageProcessing class
ImageProcessor::ImageProcessor(): local_map(100,100) {

    vid_cap = cv::VideoCapture(0); // need to check what 0 is and is not sketchy
    if(!vid_cap.isOpened()) {
        // do something
    }

    FEATURE_AREA_THRESH = 100;

    WALL_LINE_WIDTH = 2;

    BLUE_LINE_TOLERANCE = 10;

    //local_map = GridMap(600,600);

}


bool ImageProcessor::contour2small(std::vector<cv::Point>& contour) {
    return (cv::contourArea(contour,0) < FEATURE_AREA_THRESH);
}

// remove contours based on area
// do i want it based on color???
// pass variables down
// --> different feature area threshold for different cases
// also depend on hierarchy??

// VERY INEFFICIENT
void ImageProcessor::cleanContour(ImageProcessor::ContourData& contour_data) {
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

// im_src is gray/binary
ImageProcessor::ContourData ImageProcessor::getContours(cv::Mat& im_src) {
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Mat im_src_copy;
    im_src.copyTo( im_src_copy);
    cv::findContours(im_src_copy, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    ImageProcessor::ContourData contour_data;
    contour_data.contours = contours;
    contour_data.hierarchy = hierarchy;
    cleanContour(contour_data);
    cv::Mat drawing = cv::Mat::zeros(im_src.size(), CV_8UC3);
    for(int i = 0; i < contours.size(); i++) {
        cv::drawContours(drawing, contour_data.contours, -1, cv::Scalar(255,255,255), 3, 8, contour_data.hierarchy, 0, cv::Point()); // use defaults
    }
    cv::cvtColor(drawing,drawing,CV_BGR2GRAY);

    contour_data.drawing = drawing;

    return contour_data;

}

void ImageProcessor::detectWall(cv::Mat& frame) {

    // detect blue line
    WallDetection::topWallLine(frame, local_map, ColorDetection::COLOR_LINE_BLUE);

    // detect yellow line
    // WallDetection::topWallLine(frame, local_map, ColorDetection::COLOR_LINE_YELLOW);

    // detect white wall + blue line

}

void ImageProcessor::detectBlocks(cv::Mat& frame) {

    // first find both colors??





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
    while(1) {
        local_map.setZeros();
        local_map.setVal(30,30,180);
        cv::Mat frame;
        vid_cap >> frame; // get a new frame from camera
        cv::imshow("raw",frame);

        //frame = cv::imread( "images/TakeImage_60.jpg", CV_LOAD_IMAGE_COLOR ); // bgr


        // test
        detectWall(frame);


        cv::Mat local_map_im = local_map.cvtImage();

        cv::imshow("video", frame);
        cv::imshow("local map", local_map_im);
        cv::waitKey(30); // somehow must need this to work
   }
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

