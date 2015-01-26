#ifndef CAMERACONFIG_H
#define CAMERACONFIG_H

#include <cmath>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>

#include "CameraConfig.h"
#include "GridMap.h"


/*********************************
** set to 0 if running on edison *
**********************************/
static const int DEBUG = 0;
/********************************/


static const double FRAME_RESIZE_SCALE = 0.5;

static const int FRAME_SIZE_X = (640*FRAME_RESIZE_SCALE);
static const int FRAME_SIZE_Y = (480*FRAME_RESIZE_SCALE);

// translation vector of camera w.r.t to robot hook
static const double CAM_ROBOT_X = 4;
static const double CAM_ROBOT_Y = 0;

// horizontal rotation of camera w.r.t. to front direction
// counter-clockwise is positive
static const double CAM_ANGLE_HOR = 0;
// vertical rotation of camera w.r.t. horizontal plane
// downwards is positive
static const double CAM_ANGLE_VERT = (22.3 / 180 * M_PI); // in case we put it at an angle -- in radians
// height of camera
// in inches
static const double CAM_HEIGHT = 11; // inches

// camera matrix elements
static const double CAM_MAT_fx = 345.2863;
static const double CAM_MAT_cx = 148.7808;
static const double CAM_MAT_fy = 344.4604;
static const double CAM_MAT_cy = 120.2642;

static Eigen::Matrix3d ROT_MAT = ((Eigen::Matrix3d() << 1,        0,                    0,
                                                        0, cos(CAM_ANGLE_VERT), -sin(CAM_ANGLE_VERT),
                                                        0, sin(CAM_ANGLE_VERT), cos(CAM_ANGLE_VERT)).finished());

static Eigen::Matrix3d CAM_MAT = (((Eigen::Matrix3d() << (CAM_MAT_fx), 0, (CAM_MAT_cx),
                                                    0, (CAM_MAT_fy), (CAM_MAT_cy),
                                                    0, 0, 1).finished())*ROT_MAT);

static Eigen::Matrix3d CAM_MAT_INV = (CAM_MAT.inverse());


// for bgr pixels
static const int COLOR_WHITE = 255;
static const int COLOR_BLACK = 0; // for drawing map (??)

// for averaging over previous frames
static const double BLOCK_FOUND_PREVIOUS_WEIGHT = 0.8;
static const double BLOCK_COLOR_PREVIOUS_WEIGHT = 0.8;
static const double BLOCK_DIST_PREVIOUS_WEIGHT = 0.8;
static const double BLOCK_ANGLE_PREVIOUS_WEIGHT = 0.8;


// vertical pixels threshold for wall detection
static const int WALL_LINE_THRESH = 3;
// wall dimensions
static const double WALL_LINE_WIDTH = 2;
static const double WALL_HEIGHT = 6; // white + line
static const double WALL_HEIGHT_TO_LINE = 4;

// block dimensions
static const double BLOCK_HEIGHT = 2; // inches

// for block estimations
static const double COS_THRESH = 0.1;

static const int POLY_VERTEX_NUM_THRESH = 8;

static const double POLY_NEIGHBORHOOD = 9;

static const double FEATURE_AREA_THRESH = 200;

static const double ASPECT_RATIO_LOW = 0.8;
static const double ASPECT_RATIO_UP = 4;

// image utils parameters
// for gaussian blur
static const int SMOOTH_KERNEL = 3;
static const int MORPH_KERNEL = 3;
// for canny edge detection
static const int CANNY_THRESH_LOW = 5;
static const int CANNY_THRESH_UP = 15;
static const int CANNY_KERNEL = 3;
// for hough line detection (standard)
static const double HOUGH_RES_RHO_PIXEL = 1;
static const double HOUGH_RES_THETA_RAD = (M_PI/180.0);
static const int HOUGH_MIN_VOTES = 0;
// for hough line detection (probabilistic)
static const double HOUGH_P_RES_RHO_PIXEL = 1;
static const double HOUGH_P_RES_THETA_RAD = (M_PI/180.0);// 1 radian
static const int HOUGH_P_MIN_VOTES = 100;
static const int HOUGH_P_MAX_LINE_GAP = 5;




/*
// with rotation
static Eigen::Matrix3d CAM_MAT((Eigen::Matrix3d() << (FRAME_RESIZE_SCALE*685), (FRAME_RESIZE_SCALE*115.7287), (FRAME_RESIZE_SCALE*271.3519), 0, (FRAME_RESIZE_SCALE*720.8317), (FRAME_RESIZE_SCALE*(-50.8599)), 0, (FRAME_RESIZE_SCALE*0.3923), (FRAME_RESIZE_SCALE*0.9198)).finished());
static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << (0.00145/FRAME_RESIZE_SCALE), 0, ((-0.43065)/FRAME_RESIZE_SCALE), 0, (0.0013346/FRAME_RESIZE_SCALE), (0.074465/FRAME_RESIZE_SCALE), 0, ((-0.0005743)/FRAME_RESIZE_SCALE), (1.055390/FRAME_RESIZE_SCALE)).finished());
// without rotation
//static Eigen::Matrix3d CAM_MAT((Eigen::Matrix3d() << 685.3, 0, 295.5, 0, 683.9, 236.4, 0, 0, 1).finished());
//static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << 0.00145985,0,-0.430656,0,0.00146412,-0.34553440,0,0,1).finished());
*/

// update rate for camera thread
static const int UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS = 100;

#endif
