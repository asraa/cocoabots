#ifndef CAMERACONFIG_H
#define CAMERACONFIG_H
static const int DEBUG = 0;

// translation vector of camera w.r.t to robot center
static const double CAM_ROBOT_X = 0;
static const double CAM_ROBOT_Y = 0;

// counter-clockwise is positive
static const double CAM_ANGLE_HOR = 0;
// downwards is positive
static const double CAM_ANGLE_VERT = 10; // in case we put it at an angle -- in degrees
// in inches
static const double CAM_HEIGHT = 5; // don't know yet

// needs to incorporate rotation
// problematic, come back to later
static Eigen::Matrix3d CAM_MAT((Eigen::Matrix3d() << 685.3, 0, 295.5, 0, 683.9, 236.4, 0, 0, 1).finished());
//static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << 0.00145985,0,-0.430656,0,0.00146412,-0.34553440,0,0,1).finished());
static Eigen::Matrix3d CAM_MAT_INV((Eigen::Matrix3d() << 0.00145985,0,-0.430656,0,0.0014142,-0.0749415,0,-0.00037894,1.05535671).finished());

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


static const int UPDATE_RATE_IMAGE_PROCESSOR_MICROSECONDS = 40000;
#endif
