/*
 * void updateCoords(Eigen::Vector2d block_cam_coord) {
 *  double x_cam = block_cam_coord.at(0);
 *  double y_cam = block_cam_coord.at(1);
 *  double x_cam' = x * cos(CAM_ANGLE_HOR) - y * sin(CAM_ANGLE_HOR);
 *  double y_cam' = x * sin(CAM_ANGLE_HOR) + y * cos(CAM_ANGLE_HOR);
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

%%
CAM_ROBOT_X = 3.8;
CAM_ROBOT_Y = 2;
CAM_ANGLE_HOR = -1/180*pi;
x_cam = 2.3;
y_cam = 21.53;
x_cam2 = x_cam * cos(CAM_ANGLE_HOR) - y_cam * sin(CAM_ANGLE_HOR);
y_cam2 = x_cam * sin(CAM_ANGLE_HOR) + y_cam * cos(CAM_ANGLE_HOR);

x_rob = x_cam2 + CAM_ROBOT_X
y_rob = y_cam2 + CAM_ROBOT_Y

dist = norm([x_rob,y_rob])
phi = asin(x_rob/dist) / pi * 180
