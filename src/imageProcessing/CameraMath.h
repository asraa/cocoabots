#ifndef CAMERAMATH_H
#define CAMERAMATH_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/LU>
#include <iostream>
#include <math.h>
#include "CameraConfig.h"

namespace CameraMath {

double determineDepth(double, double);
//Eigen::Vector2d reconstructPoint2D(Eigen::Vector2d&, double, Eigen::Matrix3d&);
Eigen::Vector2d reconstructPoint2D(int, double);
Eigen::Vector2d reconstructPoint2D(Eigen::Vector2d& pt_im, double y_actual);
Eigen::Vector2d reconstructPoint2D(cv::Point pt_im, double y_actual);

Eigen::Vector2d cvtCamXY2RobotRadial(double x_cam, double y_cam);

}

#endif // CAMERAMATH_H



/*
 *
 * suppose center of robot to camera is (x0,y0,theta)
 * first rotate (x,z) coord of camera to (x',y')
 * (x'+x0, y'+y0)
 * distance r -- check
 * angle phi -- check
 * */

/*
 * void updateCoords(Eigen::Vector2d block_cam_coord) {
 *  double x_cam = block_cam_coord.at(0);
 *  double y_cam = block_cam_coord.at(1);
 *  double x_cam' = x_cam * cos(CAM_ANGLE_HOR) + y_cam * sin(CAM_ANGLE_HOR);
 *  double y_cam' = - x_cam * sin(CAM_ANGLE_HOR) + y_cam * cos(CAM_ANGLE_HOR);
 *
 *  double x_robot = x_cam' + CAM_ROBOT_X;
 *  double y_robot = y_cam' + CAM_ROBOT_Y;
 *
 *  double dist = sqrt(x_robot*x_robot + y_robot*y_robot);
 *  nearestCubeDistance = dist;
 *  double sin_phi = - x_robot / dist; // need to check
 *  nearestCubeAngle = asin(sin_phi);
 *
 * }
*/
