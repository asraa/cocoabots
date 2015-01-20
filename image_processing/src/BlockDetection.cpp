
#include "BlockDetection.h"

namespace BlockDetection {

// a lot of this depends on good color thresholding.
// --> get good color profile sooooooooooooon

bool isBlock(std::vector<cv::Point> & contour) {
    double perimeter = cv::arcLength(contour, 1); // 1 for closed
    cv::Rect boundRect = cv::boundingRect(cv::Mat(contour));

    std::vector<cv::Point> contours_poly;
    cv::approxPolyDP(cv::Mat(contour),contours_poly, POLY_NEIGHBORHOOD, true);
    if(contours_poly.size() > 8)
        return false;
    return (2*boundRect.height+2*boundRect.width > perimeter);
}

// simple case for now:
// single block, no occlusion

// omits case of occlusion
// will come back to occlusion problem later <-- very interesting
int numOfBlocksEst(std::vector<cv::Point> & contours_poly){

    cv::Rect boundRect = cv::boundingRect(cv::Mat(contours_poly));
    double asp_rat = ((double) boundRect.height )/ boundRect.width; // check if these are double or int
    if(asp_rat < 1.5) return 1;
    else if (asp_rat <2.5) return 2;
    else return 3; // substitute magical numbers later

}

// very crude estimate just using top most point - assumes it's on the top of the block
// does not consider occlusion
Eigen::Vector2d crudeEstimate(std::vector<cv::Point> & contour) {

    std::vector<cv::Point> contours_poly;
    cv::approxPolyDP(cv::Mat(contour),contours_poly, POLY_NEIGHBORHOOD, true);

    // estimate number of blocks in stack
    int num_of_blocks = numOfBlocksEst(contours_poly);

    /*
    // find top most point
    int top_ind= 0;
    double top_y = contours_poly.at(0).y;
    for(int j = 0; j < contours_poly.size(); j++) {
        cv::Point pt_temp = contours_poly.at(j);
        std::cout<<pt_temp<<std::endl;
        if(pt_temp.y < top_y) {
            top_ind = j;
            top_y = pt_temp.y;
        }
    }
    cv::Point top_pt = contours_poly.at(top_ind);
    std::cout<<top_pt<<std::endl;
*/
    // **** until i figure out camera matrix problem ****
    for(int j = 0; j < contours_poly.size(); j++) {
        cv::Point pt1, pt2;
        if(j<contours_poly.size()-1){
            pt1 = contours_poly.at(j);
            pt2 = contours_poly.at(j+1);
        } else {
            pt1 = contours_poly.at(j);
            pt2 = contours_poly.at(0);
        }
        double delta_x = pt2.x - pt1.x;
        double delta_y = pt2.y - pt1.y;

        double dist = sqrt(delta_x*delta_x + delta_y*delta_y);
        double cos_theta = delta_x/dist;
        if(fabs(cos_theta) < 0.1) { // magic number
            double s = CameraMath::determineDepth(dist, num_of_blocks * BLOCK_HEIGHT);
            Eigen::Vector2d pt_cam = CameraMath::reconstructPoint2D(pt1.x,s);
            return pt_cam;
        }
    }

    // return (x,z) coordinate of block
    //return CameraMath::reconstructPoint2D(top_pt, num_of_blocks * BLOCK_HEIGHT);

}


// crude
/*
Eigen::Vector2d crudeEstimate(std::vector<cv::Point> & contour) {

    std::vector<cv::Point> contours_poly;
    cv::approxPolyDP(cv::Mat(contour),contours_poly, POLY_NEIGHBORHOOD, true);

    cv::Point pt_im1;
    cv::Point pt_im2;

    std::cout << "size"<<contours_poly.size()<<" " <<contour.size();

    int n = 0;

    for(int j = 0; j < contours_poly.size(); j++) {
        cv::Point pt1, pt2;
        if(j < contours_poly.size() - 1) {
            pt1 = contours_poly.at(j);
            pt2 = contours_poly.at(j+1);
        } else { // wrap around
            pt1 = contours_poly.at(j);
            pt2 = contours_poly.at(0);
        }

        double delta_x = pt2.x - pt1.x;
        double delta_y = pt2.y - pt1.y;

        double dist = sqrt(delta_x*delta_x + delta_y*delta_y);
        double cos_theta = delta_x/dist;
        if(dist > BLOCK_THRESH && fabs(cos_theta) < COS_THRESH) {
            if(pt1.y < pt2.y) {
                if(n==0)
                    pt_im1 = pt1;
                else
                    pt_im2 = pt1;
            }
            else {
                if(n==0)
                    pt_im1 = pt2;
                else
                    pt_im2 = pt2;
            }
            n = n+1;
        }
    }
    std::cout<<pt_im1<<"dd"<<pt_im2;
    Eigen::Vector2d pt_c1 = CameraMath::reconstructPoint2D(pt_im1, BLOCK_HEIGHT);
    Eigen::Vector2d pt_c2 = CameraMath::reconstructPoint2D(pt_im2, BLOCK_HEIGHT);
    std::cout<<"camer"<<pt_c1<<" "<<pt_c2;

    return (pt_c1 + pt_c2) / 2;
}
*/

// only works for one stack/block right now
// red blocks
// TO-DO: REORGANIZE CODE
void detectBlock(cv::Mat& frame, int& found_cube, double& nearest_cube_angle, double& nearest_cube_dist, int& nearest_cube_color) {

    cv::Mat im_red = ColorDetection::detectColor(frame, ColorDetection::COLOR_BLOCK_RED);

    if(DEBUG == 1){
        cv::namedWindow("ae",1);
        cv::imshow("ae",im_red);
    }

    //ImageUtils::binaryImagePreProcess(im_red, cv::MORPH_CLOSE);

    ContourUtils::ContourData contour_data = ContourUtils::getContours(im_red);

    if(DEBUG==1){
        cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);
        cv::drawContours(drawing, contour_data.contours, -1, cv::Scalar(255,255,255), 1, 8);
        cv::namedWindow("qq",1);
        cv::imshow("qq",drawing);
    }

    ContourUtils::cleanContour(contour_data); // remove small features

    if(DEBUG==1) {
        cv::Mat drawing2 = cv::Mat::zeros(frame.size(), CV_8UC3);
        cv::drawContours(drawing2, contour_data.contours, -1, cv::Scalar(255,255,255), 1, 8);
        cv::namedWindow("dd",1);
        cv::imshow("dd",drawing2);
    }

    if(contour_data.contours.size() < 1) {
        updateBlockNotFound(found_cube);
    } else { // need to change later
        // assuming the contours are sorted from top to bottom
        // NEED TO CHECK
        for(int j = contour_data.contours.size()-1; j > -1; j--) {
            if(isBlock(contour_data.contours.at(j))) {
                Eigen::Vector2d result = crudeEstimate(contour_data.contours.at(j)); // hacked for now
                updateBlockFoundInfo(result, 1, found_cube, nearest_cube_angle, nearest_cube_dist, nearest_cube_color);
                if(DEBUG==1) {
                    std::cout<<"cube found: "<<result<<std::endl;
                }
                return; // break out of function
            }
        }
    }
    updateBlockNotFound(found_cube);

    cv::Mat im_green = ColorDetection::detectColor(frame, ColorDetection::COLOR_BLOCK_GREEN);

    if(DEBUG == 1){
        cv::namedWindow("app",1);
        cv::imshow("app",im_green);
    }

    //ImageUtils::binaryImagePreProcess(im_red, cv::MORPH_CLOSE);

    contour_data = ContourUtils::getContours(im_green);

    if(DEBUG==1){
        cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);
        cv::drawContours(drawing, contour_data.contours, -1, cv::Scalar(255,255,255), 1, 8);
        cv::namedWindow("qq2",1);
        cv::imshow("qq2",drawing);
    }

    ContourUtils::cleanContour(contour_data); // remove small features

    if(DEBUG==1) {
        cv::Mat drawing2 = cv::Mat::zeros(frame.size(), CV_8UC3);
        cv::drawContours(drawing2, contour_data.contours, -1, cv::Scalar(255,255,255), 1, 8);
        cv::namedWindow("dd2",1);
        cv::imshow("dd2",drawing2);
    }

    if(contour_data.contours.size() < 1) {
        updateBlockNotFound(found_cube);
    } else { // need to change later
        // assuming the contours are sorted from top to bottom
        // NEED TO CHECK
        for(int j = contour_data.contours.size()-1; j > -1; j--) {
            if(isBlock(contour_data.contours.at(j))) {
                Eigen::Vector2d result = crudeEstimate(contour_data.contours.at(j)); // hacked for now
                updateBlockFoundInfo(result, 0, found_cube, nearest_cube_angle, nearest_cube_dist, nearest_cube_color);
                if(DEBUG==1) {
                    std::cout<<"cube found: "<<result<<std::endl;
                }
                return; // break out of function
            }
        }
    }
    updateBlockNotFound(found_cube);

}

void updateBlockFoundInfo(Eigen::Vector2d block_coord_cam, int cube_color, int& found_cube, double& nearest_cube_angle, double& nearest_cube_dist, int& nearest_cube_color) {

    found_cube = 1;

    double x_cam = block_coord_cam[0];
    double y_cam = block_coord_cam[1];

    Eigen::Vector2d block_coord_rob_radial = CameraMath::cvtCamXY2RobotRadial(x_cam, y_cam);
    nearest_cube_dist = block_coord_rob_radial[0];
    nearest_cube_angle = block_coord_rob_radial[1];

    std::cout << "dist" << nearest_cube_dist <<"angle"<<nearest_cube_angle<<std::endl;

    nearest_cube_color = cube_color;

}

void updateBlockNotFound(int& found_cube) {
    found_cube = 0;
}

}
