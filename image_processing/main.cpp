#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>

/********************************
 // this needs to be cleaned up //
 ********************************/
/*
double CAMERA_HEIGHT = 0; 
double CAMERA_ANGLE = 0; // 0 is positive, pointing downwards in positive
double CAMERA_FOCAL_LENGTH = 2.3; //mm NEED TO CHANGE
// for 640*480 image, NEED TO SCALE ACCORDINGLy
Eigen::Matrix3d CAM_MAT;
Eigen::Matrix3d CAM_MAT_INV; 
double CAM_MAT_INV_22 = 0.0015; // for quicker height computation(necessary?)

double TOP_LINE_WIDTH = 2.0; //inches
double WALL_HEIGHT = 6.0; //inches, with blue line

double BLOCK_SIZE = 1; //inches
double BLOCK_HOLE_RADIUS = 0.5; //guess NEED TO CHANGE

int CANNY_LOW_THRESHOLD = 5;
int CANNY_UP_RATIO = 3;
int CANNY_KERNEL = 3;

int SMOOTH_KERNEL = 5;

int MORPH_KERNEL = 3;

int BLUE_LINE_TOLERANCE = 15; // random

int HSV_BLUE_H_LOW = 100;
int HSV_BLUE_H_UP = 130;
int HSV_BLUE_S_LOW = 100;
int HSV_BLUE_S_UP = 255;
int HSV_BLUE_V_LOW = 0;
int HSV_BLUE_V_UP = 255;
*/


/********************************************
 *************MAP CLASS**********************
 ********************************************/

class Map 
{
public:
  Map();
  Map(int, int);
  ~Map();

  //  Data grid_data;
  int size_x, size_y;
  int *grid_data;
  void setVal(int,int,int);
  int getSizeX();
  int getSizeY();
  int getVal(int,int);
  cv::Mat cvtImage();
};

// constructor
Map::Map() {

}

Map::Map(int size_x, int size_y) {
  this->size_x = size_x;
  this->size_y = size_y;
  grid_data = new int[size_x, size_y];

  // initialize to 0
  std::fill(grid_data, grid_data+sizeof(grid_data),0);
}

// deconstructor
Map::~Map() {
  delete grid_data;
}

// set grid data value
void Map::setVal(int x, int y, int val) {
  if(x < size_x && x > -1 && y < size_y && y > -1) {
    grid_data[x*size_x + y];
  }
}

// get grid data value
int Map::getVal(int x, int y) {

}

// get dimensions
int Map::getSizeX() {
  return size_x;
}
int Map::getSizeY() {
  return size_y;
}

// convert into Mat image for display -- debugging purposes
cv::Mat Map::cvtImage() {

}

/***********************************************
 ***********IMAGE PROCESSING CLASS**************
 **********************************************/

class ImageProcessor 
{
public:
  ImageProcessor();
  
  cv::VideoCapture vid_cap;
  
  Eigen::Matrix3d CAM_MAT; // recalibrate?
  Eigen::Matrix3d CAM_MAT_INV;  
  
  int COLOR_LINE_BLUE;
  int COLOR_LINE_RED;
  int COLOR_LINE_YELLOW;
  int COLOR_WALL;
  int COLOR_BLOCK_RED;
  int COLOR_BLOCK_GREEN;

  Map local_map; // for now

  void binaryImagePreProcess(cv::Mat&);
  cv::Mat detectColor(cv::Mat&, int*, int);
  cv::Mat detectColor(cv::Mat&, int);
  cv::Mat detectWall(cv::Mat&);
  void determineDepth(double, double); // NEED TO CHANGE

  void run(void);
};


// initializer for ImageProcessing class
ImageProcessor::ImageProcessor() {

  vid_cap = cv::VideoCapture(0); // need to check what 0 is and is not sketchy
  if(!vid_cap.isOpened()) {
    // do something
  }
  
  CAM_MAT << 685.3, 0, 295.5, 0, 683.9, 236.4, 0, 0, 1; 
  CAM_MAT_INV = CAM_MAT.inverse();

  COLOR_LINE_BLUE = 1;
  COLOR_LINE_RED = 2;
  COLOR_LINE_YELLOW = 3;
  COLOR_WALL = 4;
  COLOR_BLOCK_RED = 5;
  COLOR_BLOCK_GREEN = 6;

  local_map = Map(100,100);

 }

// dectect colors
// takes in colors array to account or different combinations of colors
// e.g. blue & white --> wall / red & green --> stacks of blocks
cv::Mat ImageProcessor::detectColor(cv::Mat& frame, int colors[], int size_of_colors){  

  cv::Mat result;

  // temp color detection
  cv::Mat frame_hsv;
  cv::cvtColor(frame, frame_hsv, CV_BGR2HSV);

  for(int i = 0; i < size_of_colors; i++) {
    result = detectColor(frame, colors[i]);
  }
  return result;

}

cv::Mat ImageProcessor::detectColor(cv::Mat& frame, int color){
  cv::Mat result;
  // hacking hsv values for now
  if(color == COLOR_LINE_BLUE)
    inRange(frame, cv::Scalar(100,100,100), cv::Scalar(130,255,255), result);
  return result;
}

void ImageProcessor::binaryImagePreProcess(cv::Mat& frame){

  int SMOOTH_KERNEL = 3;
  int MORPH_KERNEL = 3;

  // smooth the image
  cv::blur(frame, frame, cv::Size(SMOOTH_KERNEL,SMOOTH_KERNEL));
  // remove random bits
  cv::Mat clean_kernel = cv::Mat::ones(MORPH_KERNEL, MORPH_KERNEL, CV_8U);
  cv::morphologyEx(frame, frame, cv::MORPH_OPEN, clean_kernel);

}

/*
void ImageProcessor::cleanContours(){

}*/

cv::Mat ImageProcessor::detectWall(cv::Mat& frame) {
  // get blue color

  // binary image pre process

  // canny edge detector

  // sketchy method to discard stuff etc.

  // use white wall to get slopes?

  // get depth
}

// returns z w.r.t camera reference frame
// _im denotes image coords, _c denotes camera ref system coords
void ImageProcessor::determineDepth(double delta_y_im, double real_height) {
  
  //  double s = TOP_LINE_WIDTH / (CAM_MAT_INV_22 * line_width[j]);
  //  Eigen::Vector3d pt_im;
  //  pt_im << j, line_bottom[j], 1;
  //  Eigen::Vector3d pt_c = (CAM_MAT_INV * pt_im) * s; 

}
/*
Eigen::Vector3d ImageProcessor::determine3dCoords() {

}
*/

void ImageProcessor::run() {
  cv::namedWindow("video",1); // what's the 1 for?
  cv::namedWindow("local map",1);
  while(1) {
    cv::Mat frame;
    //vid_cap >> frame; // get a new frame from camera

    frame = cv::imread( "images/TakeImage_60.jpg", CV_LOAD_IMAGE_COLOR ); // bgr

    binaryImagePreProcess(frame);

    // test
    cv::Mat blue;
    blue = detectColor(frame, COLOR_LINE_BLUE);

    //    detectBlueLines(frame);

    //detectBlueLines(frame);

    //cv::Mat local_map_im = local_map.cvtImage();

    cv::imshow("video", frame);
    cv::imshow("local map", blue);
    cv::waitKey(30); // somehow must need this to work
  }
}

int main(int argc, char** argv) {
  ImageProcessor image_process;
  image_process.run();
}
