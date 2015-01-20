
#include "WallDetection.h"

// lol stupid bug - WallDection =P
namespace WallDetection {

// returns binary image of line color
cv::Mat getWallLine(cv::Mat& im_src, int color) {
    cv::Mat im_color;
    im_color = ColorDetection::detectColor(im_src, color);
    ImageUtils::binaryImagePreProcess(im_color, cv::MORPH_OPEN);
    return im_color;
}

// ** changes the source image
// scans through each line
int * scanLine(cv::Mat& im_src, cv::Mat& im_line_edges) {

    // might want to save num_rows and num_cols as const or something
    // actually don't need line_width
    int num_rows = im_src.rows;
    int num_cols = im_src.cols;
    int line_bottom [num_cols];
    std::fill(line_bottom, line_bottom + num_cols, -1);
    int line_top [num_cols];
    std::fill(line_top, line_top + num_cols, -1);
    for (int i=num_rows-1; i>-1; i=i-1) { // from bottom to top
        for (int j=0; j<num_cols; j++) {
            if(line_top[j] == -1) { // line has not been detect yet
                uchar tmp = im_line_edges.at<uchar>(i,j); // get gray value
                if(tmp == COLOR_WHITE) {
                    if(line_bottom[j] == -1) // no edge has been found before
                        line_bottom[j] = i;
                    else {
                        if(std::abs(i - line_bottom[j]) > WALL_LINE_THRESH) {
                            line_top[j] = i;
                        } else
                            line_bottom[j] = i;
                    }
                }
            } else if(line_top[j]!=-1) { // maybe move this into a separate function
                cv::Vec3b blackPixel(0,0,0);
                im_src.at<cv::Vec3b>(i,j) = blackPixel; //modify source image to omit stuff above blue line
            }
        }
    }

    int *result = new int[2*num_cols];
    for(int i=0; i<num_cols; i++) {
        result[2*i] = line_bottom[i];
        result[2*i+1] = line_top[i];
    }

    return result; // what's up with this -- gives warning.
}


/*
void updateMap(GridMap& local_map, int line_result, int size){

    for(int i = 0; i<size; i++) {
        if(line_result[3*i+2] != -1) {
            double s = CameraMath::determineDepth(-line_width[j], WALL_LINE_WIDTH, CAM_MAT_INV);
            Eigen::Vector2d pt_im;
            pt_im << j, line_bottom[j];
            Eigen::Vector2d pt_c = CameraMath::reconstructPoint2D(pt_im, s, cam_mat_inv);

            // update map
            int x = floor(30+pt_c[0]);
            int y = floor(30+pt_c[1]);
            local_map.setVal(x,y,255);
        }
    }

}
*/
void updateMapPts(GridMap& local_map, int field_object, double pts_im[], int no_of_pts) {

    // define field_object later
    for(int i = 0; i < 2*no_of_pts; i++) {
        int x = floor(pts_im[2*i]);
        int y = floor(pts_im[2*i+1]);
        local_map.setVal(x,y,255);
    }

}

void updateMapPts(GridMap& local_map, int * scan_line_output, int num_cols){
    for(int j = 0; j < num_cols; j++) {
        Eigen::Vector2d pt_c;
        // ************** until i figure out camera matrix math ***********************
        /*        if(scan_line_output[2*j+1] != -1) { // line top exists
            Eigen::Vector2d pt_im;
            pt_im << j, scan_line_output[2*j+1];
            pt_c = CameraMath::reconstructPoint2D(pt_im, WALL_HEIGHT);
            //double s = CameraMath::determineDepth(-scan_line_output[2*j], 4); // hard code 4
            //Eigen::Vector2d pt_c = CameraMath::reconstructPoint2D(j, s);
        } else if (scan_line_output[2*j] != -1) {
            Eigen::Vector2d pt_im;
            pt_im << j, scan_line_output[2*j+1];
            pt_c = CameraMath::reconstructPoint2D(pt_im, WALL_HEIGHT_TO_LINE);
        }

        // update map
        int x = floor(30+pt_c[0]); // hacking for now
        int y = floor(30+pt_c[1]);
        local_map.setVal(x,y,255);
        */
        if(scan_line_output[2*j+1] != -1) { // line top exists
            double s = CameraMath::determineDepth(-scan_line_output[2*j], 4); // hard code 4
            Eigen::Vector2d pt_c = CameraMath::reconstructPoint2D(j, s);


            // update map
            int x = floor(30+pt_c[0]); // hacking for now
            int y = floor(30+pt_c[1]);
            local_map.setVal(x,y,255);
        }
    }
}

// should it be const
void detectWall(cv::Mat& frame, GridMap& local_map, int color) {

    cv::Mat im_color = WallDetection::getWallLine(frame, color);
    cv::Mat color_edges = ImageUtils::cannyEdge(im_color);
    int *line_pts = WallDetection::scanLine(frame, color_edges);
    WallDetection::updateMapPts(local_map, line_pts, frame.cols);
    delete line_pts;
}




}
