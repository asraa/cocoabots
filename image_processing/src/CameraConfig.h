#ifndef CAMERACONFIG_H
#define CAMERACONFIG_H

/*********************************
** set to 0 if running on edison *
**********************************/
static const int DEBUG = 1;
/********************************/


/********************************************************
 * NEED TO DIVIDE THINGS BY RESCALE FACTOR IF RESCALE ***
 * *****************************************************/

// translation vector of camera w.r.t to robot center
static const double CAM_ROBOT_X = 4;
static const double CAM_ROBOT_Y = 0;

// horizontal rotation of camera w.r.t. to front direction
// counter-clockwise is positive
static const double CAM_ANGLE_HOR = 0;
// vertical rotation of camera w.r.t. horizontal plane
// downwards is positive
static const double CAM_ANGLE_VERT = 23.0977; // in case we put it at an angle -- in degrees
// height of camera
// in inches
static const double CAM_HEIGHT = 11; // don't know yet

/*************** CAMERA MATRIX *********************/
// with rotation
static Eigen::Matrix3d CAM_MAT((Eigen::Matrix3d() << 685, 115.7287, 271.3519, 0, 720.8317, -50.8599, 0, 0.3923, 0.9198).finished());
static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << 0.00145, 0, -0.43065, 0, 0.0013346, 0.074465, 0, -0.0005743, 1.055390).finished());
// without rotation
//static Eigen::Matrix3d CAM_MAT((Eigen::Matrix3d() << 685.3, 0, 295.5, 0, 683.9, 236.4, 0, 0, 1).finished());
//static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << 0.00145985,0,-0.430656,0,0.00146412,-0.34553440,0,0,1).finished());
/***************************************************/

static const int FRAME_SIZE_X = 640;
static const int FRAME_SIZE_Y = 480;


// **** DEPRECATED **********************************
// camera matrix elements
static const double CAM_MAT_fx = 685.3;
static const double CAM_MAT_cx = 295.5;
static const double CAM_MAT_fy = 683.9;
static const double CAM_MAT_cy = 236.4;

// inverse camera matrix elements
static const double CAM_MAT_INV_11 = 0.00145985;
static const double CAM_MAT_INV_13 = -0.430656;
static const double CAM_MAT_INV_22 = 0.00146412;
static const double CAM_MAT_INV_23 = -0.34553440;
// **************************************************

// update rate for camera thread
static const int UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS = 10000;

#endif
