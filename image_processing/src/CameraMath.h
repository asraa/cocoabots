#ifndef CAMERAMATH_H
#define CAMERAMATH_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <math.h>

namespace CameraMath {

//const double B_data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
//Eigen::Matrix3d B(B_data);

// needs to incorporate rotation
static Eigen::Matrix3d CAM_MAT((Eigen::Matrix3d() << 685.3, 0, 295.5, 0, 683.9, 236.4, 0, 0, 1).finished());
//static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << 0.00145985,0,-0.430656,0,0.00146412,-0.34553440,0,0,1).finished());

static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << 0.00145985,0,-0.430656,0,0.0014142,-0.0749415,0,-0.00037894,1.05535671).finished());

static const double CAM_MAT_fx = 685.3;
static const double CAM_MAT_cx = 295.5;
static const double CAM_MAT_fy = 683.9;
static const double CAM_MAT_cy = 236.4;

static const double CAM_MAT_INV_11 = 0.00145985;
static const double CAM_MAT_INV_13 = -0.430656;
static const double CAM_MAT_INV_22 = 0.00146412;
static const double CAM_MAT_INV_23 = -0.34553440;

static const double CAM_ANGLE = 10; // in case we put it at an angle -- in degrees
static const double CAM_HEIGHT = 5; // don't know yet

double determineDepth(double, double);
//Eigen::Vector2d reconstructPoint2D(Eigen::Vector2d&, double, Eigen::Matrix3d&);
Eigen::Vector2d reconstructPoint2D(int, double);
Eigen::Vector2d reconstructPoint2D(Eigen::Vector2d& pt_im, double y_actual);
Eigen::Vector2d reconstructPoint2D(cv::Point pt_im, double y_actual);

}

#endif // CAMERAMATH_H
