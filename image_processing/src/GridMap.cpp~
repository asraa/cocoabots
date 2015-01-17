#include "GridMap.h"

// constructor
GridMap::GridMap(int size_x, int size_y) {
  this->size_x = size_x;
  this->size_y = size_y;
  grid_data = new int[size_x*size_y];

  // initialize to 0
  std::fill(grid_data, grid_data+size_x*size_y, 0);
}

// deconstructor
GridMap::~GridMap() {
  delete grid_data;
}

// set grid data value
void GridMap::setVal(int x, int y, int val) {
  if(x < size_x && x > -1 && y < size_y && y > -1) {
    grid_data[y*size_x + x]=val;
  }
}

void GridMap::setZeros() {
  std::fill(grid_data, grid_data+size_x*size_y, 0);
}

// get grid data value
int GridMap::getVal(int x, int y) {
  return grid_data[y*size_x + x];
}

// get dimensions
int GridMap::getSizeX() {
  return size_x;
}
int GridMap::getSizeY() {
  return size_y;
}

// convert into Mat image for display -- debugging purposes
cv::Mat GridMap::cvtImage() {
  cv::Mat map_image = cv::Mat::zeros( size_x, size_y, CV_8UC1 );
  for(int i=0; i<size_x; i++) {
    for(int j=0; j<size_y; j++) {
      if(getVal(i,j)!=0) {uchar pp = 255;
	map_image.at<uchar>(i,j) = pp;}
    }
  }
  return map_image;
}

