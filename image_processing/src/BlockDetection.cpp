
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
    if(perimeterRatio2large(contour)){
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
    }
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
    std::cout<<bottom_pt<<std::endl;
    if(bottom_pt.y < FRAME_SIZE_Y)
        return CameraMath::reconstructPoint2D(bottom_pt, 0); // on the floor

    // otherwise find top most point
    int top_ind= findHighestPoint(contours_poly);
    cv::Point top_pt = contours_poly.at(top_ind);
    std::cout<<top_pt<<std::endl;

    // estimate number of blocks in stack
    int num_of_blocks = numOfBlocksEst(contours_poly);

    // return (x,z) coordinate of block
    return CameraMath::reconstructPoint2D(top_pt, num_of_blocks * BLOCK_HEIGHT);

    /*
    Eigen::Vector2d pt_cam;
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

        double dist = isVertical(pt1,pt2); // returns -1 if not vertical
        std::cout << "dist " << dist << std::endl;

        if(dist > 0) {
            double s = CameraMath::determineDepth(dist, num_of_blocks * BLOCK_HEIGHT);
            if(DEBUG==1) {
                std::cout << "s value " << s << std::endl;
            }
            pt_cam = CameraMath::reconstructPoint2D(pt1.x,s);
            return pt_cam;
        }
    }

    pt_cam << -1,-1;
    return pt_cam;
    */

}

// finds the lowest point in the contour
int findLowestPoint(std::vector<cv::Point> contour){
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
int findHighestPoint(std::vector<cv::Point> contour){
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
        std::cout << j << " " << findLowestPoint(contour_data.contours.at(j)) << std::endl;
    }
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

// TO-DO: REORGANIZE CODE
void detectBlocks(cv::Mat& frame, BlockInfo& nearest_block_info) {

    // for DEBUG
    clock_t start, end;
    start = clock();

    cv::Mat im_red = ColorDetection::detectColor(frame, ColorDetection::COLOR_BLOCK_RED);

    if(DEBUG == 1){
        cv::namedWindow("ae",1);
        cv::imshow("ae",im_red);
    }

    // CHECK BACK ON THIS
    //ImageUtils::binaryImagePreProcess(im_red, cv::MORPH_CLOSE);

    ContourUtils::ContourData contour_data = ContourUtils::getContours(im_red);

    if(DEBUG==1){
        cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);
        cv::drawContours(drawing, contour_data.contours, -1, cv::Scalar(255,255,255), 1, 8);
        cv::namedWindow("qq",1);
        cv::imshow("qq",drawing);
    }

/*
    // or maybe just move to isBlock or sth like that
    ContourUtils::cleanContour(contour_data); // remove small features

    if(DEBUG==1) {
        cv::Mat drawing2 = cv::Mat::zeros(frame.size(), CV_8UC3);
        cv::drawContours(drawing2, contour_data.contours, -1, cv::Scalar(255,255,255), 1, 8);
        cv::namedWindow("dd",1);
        cv::imshow("dd",drawing2);
    }
*/
    std::cout<<"number of contours"<<contour_data.contours.size()<<std::endl;
    if(contour_data.contours.size() > 0) { // contours not null
        // findLowestContour(contour_data);
        for(int j = contour_data.contours.size()-1; j > -1; j--) {
            if(isBlock(contour_data.contours.at(j))) {
                std::cout<<"hello"<<std::endl;
                // assuming sorted from top to bottom
                Eigen::Vector2d result = crudeEstimate(contour_data.contours.at(j)); // hacked for now
                std::cout << result << std::endl;
                if(result[0] != -1) {
                    updateBlockFoundInfo(result, 1, nearest_block_info);
                    if(DEBUG==1) {
                        std::cout<<"cube found: x,y"<<std::endl<<result<<std::endl;
                        if(DEBUG==1) {
                            end = clock();
                        }

                        std::cout << "block time " << ((double) (end - start)) / CLOCKS_PER_SEC << std::endl;
                    }
                    return;
                }
            }
        }
    }
    updateBlockNotFound(nearest_block_info);

    if(DEBUG==1) {
        end = clock();
    }

    std::cout << "block time " << ((double) (end - start)) / CLOCKS_PER_SEC << std::endl;
}

/*****

  What I want to do:

  detect red
  - list of possible points

  detect green
  - list of possible points

  update nearest info

  update map with global info


  */

void updateBlockFoundInfo(Eigen::Vector2d block_coord_cam, int cube_color, BlockInfo& nearest_block_info) {

    nearest_block_info.found_cube = 1;

    double x_cam = block_coord_cam[0];
    double y_cam = block_coord_cam[1];

    Eigen::Vector2d block_coord_rob_radial = CameraMath::cvtCamXY2RobotRadial(x_cam, y_cam);
    nearest_block_info.nearest_cube_dist = block_coord_rob_radial[0];
    nearest_block_info.nearest_cube_angle = block_coord_rob_radial[1];

    //std::cout << "dist" << nearest_cube_dist <<"angle"<<nearest_cube_angle<<std::endl;

    nearest_block_info.nearest_cube_color = cube_color;

}

void updateBlockNotFound(BlockInfo& nearest_block_info) {
    // set found_cube to 0 keep all other information unchanged
    nearest_block_info.found_cube = 0;
}

}
