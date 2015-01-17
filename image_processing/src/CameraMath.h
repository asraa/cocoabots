#ifndef CAMERAMATH_H
#define CAMERAMATH_H

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <math.h>

namespace CameraMath {

static const double CAM_MAT_fx = 685.3;
static const double CAM_MAT_cx = 295.5;
static const double CAM_MAT_fy = 683.9;
static const double CAM_MAT_cy = 236.4;

static const double CAM_MAT_INV_11 = 0.00145985;
static const double CAM_MAT_INV_13 = -0.430656;
static const double CAM_MAT_INV_22 = 0.00146412;
static const double CAM_MAT_INV_23 = -0.34553440;

static const double CAM_ANGLE = 0; // in case we put it at an angle
static const double CAM_HEIGHT = 3; // don't know yet

double determineDepth(double, double);
//Eigen::Vector2d reconstructPoint2D(Eigen::Vector2d&, double, Eigen::Matrix3d&);
Eigen::Vector2d reconstructPoint2D(int, double);


}

#endif // CAMERAMATH_H
