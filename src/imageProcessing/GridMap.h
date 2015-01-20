
#ifndef GRIDMAP_H
#define GRIDMAP_H

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

class GridMap 
{
public:
  GridMap();
  GridMap(int, int);
  ~GridMap();

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

#endif // GRIDMAP_H
