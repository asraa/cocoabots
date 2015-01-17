
#include "CameraMath.h"

namespace CameraMath {

double determineDepth(double delta_y_im, double real_height) {
    double s = real_height / (CAM_MAT_INV_22 * delta_y_im);
    return s;
}
/*
//if speed becomes a problem can pass matrix element instead
Eigen::Vector2d reconstructPoint2D(Eigen::Vector2d& pt_im, double s, Eigen::Matrix3d& cam_mat_inv) {
    double coord1 = (cam_mat_inv(0,0)*pt_im(0) + cam_mat_inv(0,2)*1) * s; // hard-coding this to save complex matrix multiplication
    Eigen::Vector2d pt_c;
    pt_c << coord1, s; // just outputting x,y (corresponding to x and z in space coord)
    return pt_c;
}
*/
Eigen::Vector2d reconstructPoint2D(int pt_im_x, double s) {
    double coord1 = (CAM_MAT_INV_11*pt_im_x + CAM_MAT_INV_13*1) * s; // hard-coding this to save complex matrix multiplication
    Eigen::Vector2d result;
    result << coord1, s;
    return result;
}


}
