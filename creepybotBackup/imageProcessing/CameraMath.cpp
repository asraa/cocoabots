
#include "CameraMath.h"

namespace CameraMath {

/*
double determineDepth(double delta_y_im, double real_delta_height) {
    double s = real_delta_height * cos(CAM_ANGLE_VERT/180*M_PI) / (CAM_MAT_INV_22 * delta_y_im);
    return s;
}


//if speed becomes a problem can pass matrix element instead
Eigen::Vector2d reconstructPoint2D(Eigen::Vector2d& pt_im, double s, Eigen::Matrix3d& cam_mat_inv) {
    double coord1 = (cam_mat_inv(0,0)*pt_im(0) + cam_mat_inv(0,2)*1) * s; // hard-coding this to save complex matrix multiplication
    Eigen::Vector2d pt_c;
    pt_c << coord1, s; // just outputting x,y (corresponding to x and z in space coord)
    return pt_c;
}


Eigen::Vector2d reconstructPoint2D(int pt_im_x, double s) {
    double coord1 = (CAM_MAT_INV_11*pt_im_x + CAM_MAT_INV_13*1) * s; // hard-coding this to save complex matrix multiplication
    Eigen::Vector2d result;
    result << coord1, s;
    return result;
}
*/

// returns (x,z) coordinates relative to camera
Eigen::Vector2d reconstructPoint2D(Eigen::Vector2d& pt_im, double y_actual) {
    Eigen::Vector3d pt_im_hom;
    pt_im_hom << pt_im(0), pt_im(1), 1; // homoegeneous coordinates
    Eigen::Vector3d temp = CAM_MAT_INV * pt_im_hom;
    double s = (CAM_HEIGHT - y_actual) / temp(1);
    Eigen::Vector2d result;
    result << temp(0)*s, temp(2)*s;
    return result;
}

Eigen::Vector2d reconstructPoint2D(cv::Point& pt_im, double y_actual) {
    //std::cout<<"rr" << pt_im.x <<" " <<pt_im.y<<std::endl;
    Eigen::Vector3d pt_im_hom;
    pt_im_hom << pt_im.x, pt_im.y, 1;
    Eigen::Vector3d temp = CAM_MAT_INV * pt_im_hom;
    double s = (CAM_HEIGHT - y_actual) / temp(1);
    Eigen::Vector2d result;
    result << temp(0)*s, temp(2)*s;
    return result;
}

Eigen::Vector2d cvtCamXY2RobotRadial(double x_cam, double y_cam) {
    double x_cam_rot = x_cam * cos(CAM_ANGLE_HOR) - y_cam * sin(CAM_ANGLE_HOR);
    double y_cam_rot = x_cam * sin(CAM_ANGLE_HOR) + y_cam * cos(CAM_ANGLE_HOR);

    double x_robot = x_cam_rot + CAM_ROBOT_X;
    double y_robot = y_cam_rot + CAM_ROBOT_Y;
    double dist = sqrt(x_robot*x_robot + y_robot*y_robot);
    // this is x_robot because rotation is w.r.t. to forward direction
    // clockwise is positive, counter-clockwise is negative
    double sin_phi = x_robot / dist;
    double phi = asin(sin_phi);

    if(DEBUG==1) {
        std::cout<<"cam angle"<<CAM_ANGLE_HOR<<std::endl;
        std::cout<<"robot xy"<<x_robot<<" "<<y_robot<<std::endl;
        std::cout<<"phi "<<phi/M_PI*180<<std::endl;
    }

    Eigen::Vector2d result;
    result << dist, phi/M_PI * 180;
    return result;
}

Eigen::Vector2d cvtCamXY2RobotXY(double x_cam, double y_cam) {
    double x_cam_rot = x_cam * cos(CAM_ANGLE_HOR) - y_cam * sin(CAM_ANGLE_HOR);
    double y_cam_rot = x_cam * sin(CAM_ANGLE_HOR) + y_cam * cos(CAM_ANGLE_HOR);

    double x_robot = x_cam_rot + CAM_ROBOT_X;
    double y_robot = y_cam_rot + CAM_ROBOT_Y;

    Eigen::Vector2d result;
    result << x_robot, y_robot;
    return result;
}

}
