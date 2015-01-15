#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <math.h>

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
  void setZeros();
  int getSizeX();
  int getSizeY();
  int getVal(int,int);
  cv::Mat cvtImage();
};

// constructor
Map::Map(int size_x, int size_y) {
  this->size_x = size_x;
  this->size_y = size_y;
  grid_data = new int[size_x*size_y];

  // initialize to 0
  std::fill(grid_data, grid_data+size_x*size_y, 0);
}

// deconstructor
Map::~Map() {
  delete grid_data;
}

// set grid data value
void Map::setVal(int x, int y, int val) {
  if(x < size_x && x > -1 && y < size_y && y > -1) {
    grid_data[y*size_x + x]=val;
  }
}

void Map::setZeros() {
  std::fill(grid_data, grid_data+size_x*size_y, 0);
}

// get grid data value
int Map::getVal(int x, int y) {
  return grid_data[y*size_x + x];
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
  cv::Mat map_image = cv::Mat::zeros( size_x, size_y, CV_8UC1 );
  for(int i=0; i<size_x; i++) {
    for(int j=0; j<size_y; j++) {
      if(getVal(i,j)!=0) {uchar pp = 255;
	map_image.at<uchar>(i,j) = pp;}
    }
  }
  std::cout<<(int)map_image.at<unsigned char>(1,200)<<std::endl;
  return map_image;
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
  double CAM_MAT_INV_11;
  double CAM_MAT_INV_22;

  int COLOR_LINE_BLUE;
  int COLOR_LINE_RED;
  int COLOR_LINE_YELLOW;
  int COLOR_WALL;
  int COLOR_BLOCK_RED;
  int COLOR_BLOCK_GREEN;

  double WALL_LINE_WIDTH;

  int WHITE;
  int BLACK;

  int BLUE_LINE_TOLERANCE;

  Map local_map; // for now

  void binaryImagePreProcess(cv::Mat&);
  cv::Mat detectColor(cv::Mat&, int*, int);
  cv::Mat detectColor(cv::Mat&, int);
  cv::Mat detectWall(cv::Mat&);
  double determineDepth(double, double); // NEED TO CHANGE
  Eigen::Vector3d reconstructPoint(Eigen::Vector3d, double);
  cv::Mat cannyEdge(cv::Mat&);

  void run(void);
};

// initializer for ImageProcessing class
ImageProcessor::ImageProcessor(): local_map(40,40) {

  vid_cap = cv::VideoCapture(0); // need to check what 0 is and is not sketchy
  if(!vid_cap.isOpened()) {
    // do something
  }
  
  CAM_MAT << 685.3, 0, 295.5, 0, 683.9, 236.4, 0, 0, 1; 
  CAM_MAT_INV << 0.00145985, 0, -0.430656, 0, 0.00146412, -0.34553440, 0, 0, 1;
  CAM_MAT_INV_11 = CAM_MAT_INV(0,0);
  CAM_MAT_INV_22 = CAM_MAT_INV(1,1);

  COLOR_LINE_BLUE = 1;
  COLOR_LINE_RED = 2;
  COLOR_LINE_YELLOW = 3;
  COLOR_WALL = 4;
  COLOR_BLOCK_RED = 5;
  COLOR_BLOCK_GREEN = 6;

  WALL_LINE_WIDTH = 2;

  WHITE = 255;
  BLACK = 0;

  BLUE_LINE_TOLERANCE = 10;

  //local_map = Map(600,600);

 }

// dectect colors
// takes in colors array to account or different combinations of colors
// e.g. blue & white --> wall / red & green --> stacks of blocks
cv::Mat ImageProcessor::detectColor(cv::Mat& frame, int colors[], int size_of_colors){  

  cv::Mat result;

  // temp color detection
  for(int i = 0; i < size_of_colors; i++) {
    result = detectColor(frame, colors[i]);
  }
  return result;

}

cv::Mat ImageProcessor::detectColor(cv::Mat& frame, int color){
  cv::Mat frame_hsv;
  cv::cvtColor(frame, frame_hsv, CV_BGR2HSV);

  cv::Mat result;

  // hacking hsv values for now
  //  if(color == COLOR_LINE_BLUE)
    inRange(frame_hsv, cv::Scalar(100,100,0), cv::Scalar(130,255,255), result);

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
cv::Mat ImageProcessor::cannyEdge(cv::Mat& im_src){
  cv::Mat im_result;
  int CANNY_THRESH_LOW = 5;
  int CANNY_THRESH_UP = 15;
  int CANNY_KERNEL = 3;
  cv::Canny(im_src, im_result, CANNY_THRESH_LOW, CANNY_THRESH_UP, CANNY_KERNEL); // returns gray
  return im_result;
}

cv::Mat ImageProcessor::detectWall(cv::Mat& frame) {

  cv::Mat frame_removed;
  // get blue color
  cv::Mat im_blue;
  im_blue = detectColor(frame, COLOR_LINE_BLUE);

  binaryImagePreProcess(im_blue);

  cv::Mat blue_edges;
  blue_edges = cannyEdge(im_blue);

  // TO-DO: change this with contours

  // to find width of edge
  int num_rows = blue_edges.rows;
  int num_cols = blue_edges.cols;
  int line_bottom [num_cols];
  std::fill(line_bottom, line_bottom + num_cols, -1);
  int line_top [num_cols];
  std::fill(line_top, line_top + num_cols, -1);
  int line_width [num_cols];
  std::fill(line_width, line_width + num_cols, -1);
  for (int i=num_rows-1; i>-1; i=i-1) {
    for (int j=0; j<num_cols; j++) {
      if(line_top[j] == -1){
	uchar tmp = blue_edges.at<uchar>(i,j); // gray value
	if(tmp == WHITE) {
	  if(line_bottom[j] == -1)
	    line_bottom[j] = i;
	  else {
	    if(std::abs(i - line_bottom[j]) > BLUE_LINE_TOLERANCE) {
	      line_top[j] = i;
	      line_width[j] = i - line_bottom[j];
	    } else
	      line_bottom[j] = -1;
	  }	    
	}
      } else if(line_top[j]!=-1) {
	cv::Vec3b blackPixel(0,0,0);
	frame.at<cv::Vec3b>(i,j) = blackPixel;
      }	    	  
    }
  }

  local_map.setZeros();
  for(int j = 0; j < num_cols; j++) {
    if(line_top[j] != -1) {
      double s = determineDepth(-line_width[j], WALL_LINE_WIDTH);
      Eigen::Vector3d pt_im;
      pt_im << j, line_bottom[j], 1;
      Eigen::Vector3d pt_c = reconstructPoint(pt_im, s);
      std::cout << pt_c << std::endl;

      // update map
      int x = floor(10+pt_c[0]);
      int y = floor(10+pt_c[2]);
      std::cout<<"x"<<x<<"y"<<y<<std::endl;
      local_map.setVal(x,y,255);
    }
  }

  return blue_edges;
  // use white wall to get slopes?
}

// returns z w.r.t camera reference frame
// _im denotes image coords, _c denotes camera ref system coords
double ImageProcessor::determineDepth(double delta_y_im, double real_height) {
  
  double s = real_height / (CAM_MAT_INV_22 * delta_y_im);
  std::cout<<s<<" "<<(double)CAM_MAT_INV_22<<" "<<delta_y_im<<" "<<std::endl;
  return s;

}

Eigen::Vector3d ImageProcessor::reconstructPoint(Eigen::Vector3d pt_im, double s) {
  Eigen::Vector3d pt_c = (CAM_MAT_INV * pt_im) * s; 
  return pt_c;
}
/*
Eigen::Vector3d ImageProcessor::determine3dCoords() {

}
*/

void ImageProcessor::run() {
  cv::namedWindow("video",1); // what's the 1 for?
  cv::namedWindow("local map",cv::WINDOW_NORMAL);
  while(1) {
    cv::Mat frame;
    vid_cap >> frame; // get a new frame from camera

    //frame = cv::imread( "images/TakeImage_60.jpg", CV_LOAD_IMAGE_COLOR ); // bgr

    binaryImagePreProcess(frame);

    // test
    cv::Mat blue;
    blue = detectWall(frame);

    //    detectBlueLines(frame);

    //detectBlueLines(frame);

    cv::Mat local_map_im = local_map.cvtImage();

    cv::imshow("video", frame);
    cv::imshow("local map", local_map_im);
    cv::waitKey(30); // somehow must need this to work
     }
}

int main(int argc, char** argv) {
  ImageProcessor image_process;
  image_process.run();
}
