
#include "BlockDetection.h"

namespace BlockDetection {

// a lot of this depends on good color thresholding.
// TO-DO --> get good color profile sooooooooooooon


// bound contour area to remove small bits
bool contour2small(std::vector<cv::Point>& contour) {
    return (fabs(cv::contourArea(contour,0)) < FEATURE_AREA_THRESH); // 0 for non-oriented
}

// bound perimeter size to remove weird shapes
bool perimeterRatio2large(std::vector<cv::Point>& contour) {
    double perimeter = cv::arcLength(contour, 1); // 1 for closed contour
    cv::Rect boundRect = cv::boundingRect(cv::Mat(contour));
    // rectangle perimeter larger than perimeter of object (because of triangle inequality)
    return (perimeter > 2*boundRect.height+2*boundRect.width);
}

bool aspectRatioReasonable(std::vector<cv::Point>& contour) {
    cv::Rect boundRect = cv::boundingRect(cv::Mat(contour));
    double aspect_ratio = boundRect.height / (double) boundRect.width;

    return aspect_ratio < ASPECT_RATIO_UP && aspect_ratio > ASPECT_RATIO_LOW;
}

// number of vertices of polygon
bool numberOfVerticesReasonable(std::vector<cv::Point>& contour) {
    std::vector<cv::Point> contours_poly;
    cv::approxPolyDP(cv::Mat(contour),contours_poly, POLY_NEIGHBORHOOD, true);
    //std::cout << "poly size" <<contours_poly.size() << std::endl;
    return contours_poly.size() < POLY_VERTEX_NUM_THRESH;
}

// can add to this
bool isBlock(std::vector<cv::Point>& contour) {
    if(contour2small(contour)) {
        //std::cout<<"2small";
        return false;
    }
    /*if(perimeterRatio2large(contour)){
        //std::cout<<"2large";
        return false;
    }
    if(!aspectRatioReasonable(contour)){
        //std::cout<<"2skewed";
        return false;
    }
    if(!numberOfVerticesReasonable(contour)) {
        //std::cout<<"2manyV";
        return false;
    }*/
    return true;
}

// omits case of occlusion
int numOfBlocksEst(std::vector<cv::Point> & contours_poly){
    cv::Rect boundRect = cv::boundingRect(cv::Mat(contours_poly));
    double asp_rat = ((double) boundRect.height )/ boundRect.width; // check if these are double or int
    if(asp_rat < 1.5) return 1;
    else if (asp_rat <2.5) return 2;
    else return 3; // substitute magical numbers later
}

int isVertical(cv::Point pt1, cv::Point pt2) {

    double delta_x = pt2.x - pt1.x;
    double delta_y = pt2.y - pt1.y;
    double dist = sqrt(delta_x*delta_x + delta_y*delta_y);

    double cos_theta = delta_x/dist;

    if(fabs(cos_theta) < COS_THRESH) // fabs because abs is for int
        return dist;
    else
        return -1;

}

// very crude estimate just using top most point one side of stack -- no, other method
// does not consider occlusion
Eigen::Vector2d crudeEstimate(std::vector<cv::Point> & contour) {

    std::vector<cv::Point> contours_poly;
    cv::approxPolyDP(cv::Mat(contour),contours_poly, POLY_NEIGHBORHOOD, true);

    // find bottom most point
    int bottom_ind = findLowestPoint(contours_poly);
    cv::Point bottom_pt = contours_poly.at(bottom_ind);
    if(bottom_pt.y < FRAME_SIZE_Y)
        return CameraMath::reconstructPoint2D(bottom_pt, 0.0); // on the floor

    // otherwise find top most point
    int top_ind= findHighestPoint(contours_poly);
    cv::Point top_pt = contours_poly.at(top_ind);

    // estimate number of blocks in stack
    int num_of_blocks = numOfBlocksEst(contours_poly);

    // return (x,z) coordinate of block
    return CameraMath::reconstructPoint2D(top_pt, num_of_blocks * BLOCK_HEIGHT);

}

// finds the lowest point in the contour
int findLowestPoint(std::vector<cv::Point>& contour){
    int lowest_ind = 0;
    double lowest_val = contour.at(0).y;
    for(int j = 0; j < contour.size(); j++) {
        if(contour.at(j).y > lowest_val) { // larger than because y goes down
            lowest_ind = j;
            lowest_val = contour.at(j).y;
        }
    }
    return lowest_ind;
}

// finds the highest point in the contour
int findHighestPoint(std::vector<cv::Point>& contour){
    int highest_ind = 0;
    double highest_val = contour.at(0).y;
    for(int j = 0; j < contour.size(); j++) {
        if(contour.at(j).y < highest_val) { // less than because y goes down
            highest_ind = j;
            highest_val = contour.at(j).y;
        }
    }
    return highest_ind;
}


int findLowestContour(ContourUtils::ContourData& contour_data ) {
    for(int j = 0; j < contour_data.contours.size(); j++) {
        //std::cout << j << " " << findLowestPoint(contour_data.contours.at(j)) << std::endl;
    }
}

std::vector<Eigen::Vector2d,Eigen::aligned_allocator<Eigen::Vector2d>> findBlocksSingleColor(cv::Mat& frame, int color) {

    std::vector<Eigen::Vector2d,Eigen::aligned_allocator<Eigen::Vector2d>> list_of_pts;

    cv::Mat im_color = ColorDetection::detectColor(frame, color);
    ContourUtils::ContourData contour_data = ContourUtils::getContours(im_color);

    // CHECK BACK ON THIS
    //ImageUtils::binaryImagePreProcess(im_red, cv::MORPH_CLOSE);

    // show images
    if(DEBUG == 1){
        cv::namedWindow("ae",1);
        cv::imshow("ae",im_color);

        cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);
        cv::drawContours(drawing, contour_data.contours, -1, cv::Scalar(255,255,255), 1, 8);
        cv::namedWindow("qq",1);
        cv::imshow("qq",drawing);
    }

    //std::cout<<"number of contours"<<contour_data.contours.size()<<std::endl;
    if(contour_data.contours.size() > 0) { // contours not null
        // findLowestContour(contour_data);
        for(int j = 1; j < contour_data.contours.size(); j++) {
            if(isBlock(contour_data.contours.at(j))) {
                Eigen::Vector2d block_pt_xy = crudeEstimate(contour_data.contours.at(j)); // hacked for now
                Eigen::Vector2d block_pt_radial = CameraMath::cvtCamXY2RobotRadial(block_pt_xy[0], block_pt_xy[1]);
                //std::cout << result << std::endl;
                list_of_pts.push_back(block_pt_radial);
            }
        }
    }

    return list_of_pts; // may possibly return empty list

}

Eigen::Vector2d findNearestBlockInList(std::vector<Eigen::Vector2d,Eigen::aligned_allocator<Eigen::Vector2d>>& list_of_block_pts) {
    double dist_min_val = list_of_block_pts.at(0)[0];
    int min_ind = 0;
    for(int i=0; i<list_of_block_pts.size(); i++) {
        Eigen::Vector2d pt = list_of_block_pts.at(i);
        double dist = pt[0];
        if(dist < dist_min_val) {
            dist_min_val = dist;
            min_ind = i;
        }
    }
    return list_of_block_pts.at(min_ind);
}

bool closerThan(Eigen::Vector2d& pt1, Eigen::Vector2d& pt2) {
    return pt1[0] < pt2[0];
}


void detectBlocks(cv::Mat& frame, BlockInfo& nearest_block_info) {

    // list of points is in radial coordinates
    std::vector<Eigen::Vector2d,Eigen::aligned_allocator<Eigen::Vector2d>> list_of_red = findBlocksSingleColor(frame, ColorDetection::COLOR_BLOCK_RED);
    std::vector<Eigen::Vector2d,Eigen::aligned_allocator<Eigen::Vector2d>> list_of_green = findBlocksSingleColor(frame, ColorDetection::COLOR_BLOCK_GREEN);

    if(list_of_red.size() > 0 && list_of_green.size() > 0) {
        Eigen::Vector2d nearest_pt_red = findNearestBlockInList(list_of_red);
        Eigen::Vector2d nearest_pt_green = findNearestBlockInList(list_of_green);
        Eigen::Vector2d nearest_pt_rad;
        if(closerThan(nearest_pt_red, nearest_pt_green)) {
            nearest_pt_rad = nearest_pt_red;
            updateBlockFoundInfo(nearest_pt_rad, 1, nearest_block_info); //red
        }
        else {
            nearest_pt_rad = nearest_pt_green;
            updateBlockFoundInfo(nearest_pt_rad, 0, nearest_block_info); //red
        }
    }

    else if(list_of_red.size() > 0) {
        Eigen::Vector2d nearest_pt_rad = findNearestBlockInList(list_of_red);
        updateBlockFoundInfo(nearest_pt_rad, 1, nearest_block_info); //red
    }

    else if(list_of_green.size() > 0 ) {
        Eigen::Vector2d nearest_pt_rad = findNearestBlockInList(list_of_green);
        updateBlockFoundInfo(nearest_pt_rad, 0, nearest_block_info); // green
    }

    else
        updateBlockNotFound(nearest_block_info);

}


void updateBlockFoundInfo(Eigen::Vector2d& block_coord_rad, int cube_color, BlockInfo& nearest_block_info) {

    nearest_block_info.found_cube = 1;
    nearest_block_info.nearest_cube_dist = block_coord_rad[0];
    nearest_block_info.nearest_cube_angle = block_coord_rad[1];
    nearest_block_info.nearest_cube_color = cube_color;

    if(DEBUG == 1) {
        std::cout<<"cube found: ";
        std::cout<<"cude dist "<<nearest_block_info.nearest_cube_dist<< " ";
        std::cout<<"cube angle "<<nearest_block_info.nearest_cube_angle<<" ";
        std::cout<<"cube color "<<nearest_block_info.nearest_cube_color<<std::endl;
    }

    //Eigen::Vector2d block_coord_rob_radial = CameraMath::cvtCamXY2RobotRadial(x_cam, y_cam);
    //nearest_block_info.nearest_cube_dist = block_coord_rob_radial[0];
    //nearest_block_info.nearest_cube_angle = block_coord_rob_radial[1];

    //std::cout << "dist" << nearest_block_info.nearest_cube_dist <<"angle"<<nearest_block_info.nearest_cube_angle<<std::endl;

}

void updateBlockNotFound(BlockInfo& nearest_block_info) {
    // set found_cube to 0 keep all other information unchanged
    nearest_block_info.found_cube = 0;
}

}
