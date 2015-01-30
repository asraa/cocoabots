
#include "ColorDetection.h"

namespace ColorDetection {

// dectect colors
cv::Mat detectColor(cv::Mat& frame, int color){

    // hacking hsv values for now

    cv::Mat frame_hsv;
    cv::cvtColor(frame, frame_hsv, CV_BGR2HSV);

    cv::Mat result;

    if(color == COLOR_LINE_BLUE) {
        if(!COLOR_BLUE_LINE_USE_PROF)
            inRange(frame_hsv, cv::Scalar(100,100,0), cv::Scalar(125,255,255), result);
        else
            result = detectColorProf(frame, color);
    }
    else if(color == COLOR_LINE_YELLOW) {
        inRange(frame_hsv, cv::Scalar(20, 100, 100), cv::Scalar(30, 255, 255), result);
    }
    else if (color == COLOR_BLOCK_GREEN) {
        if(!COLOR_GREEN_BLOCK_USE_PROF)
            inRange(frame_hsv, cv::Scalar(50,100,0), cv::Scalar(100,255,255), result);
        else
            result = detectColorProf(frame, color);
    }
    else if (color == COLOR_BLOCK_RED) {
        if(!COLOR_RED_BLOCK_USE_PROF) {
            inRange(frame_hsv, cv::Scalar(0,100,0), cv::Scalar(50,255,255), result);
            cv::Mat result2;
            inRange(frame_hsv, cv::Scalar(150,100,0), cv::Scalar(255,255,255), result2);
            result = result + result2;
        } else
            result = detectColorProf(frame, color);
    }
    else if (color == COLOR_LINE_PURPLE) {
        if(!COLOR_PURPLE_LINE_USE_PROF)
            inRange(frame_hsv, cv::Scalar(130,30,0), cv::Scalar(155,255,255), result);
        else
            result = detectColorProf(frame, color);
    }
    return result;
}

// opencv is bgr
cv::Mat detectColorProf(cv::Mat& frame, int color) {

    int num_rows = frame.rows;
    int num_cols = frame.cols;

    cv::Mat result = cv::Mat::zeros( num_rows, num_cols, CV_8UC1 );

    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {

            cv::Vec3b pixel = frame.at<cv::Vec3b>(i,j);
            uchar b_val = pixel.val[0];
            uchar g_val = pixel.val[1];
            uchar r_val = pixel.val[2];
            double intensity = getIntensity(r_val, g_val, b_val);

            int mask1 = 0;

            if(color == COLOR_BLOCK_RED)
                mask1 = isRedBlock(r_val, g_val, b_val, intensity);
            if(color == COLOR_BLOCK_GREEN)
                mask1 = isGreenBlock(r_val, g_val, b_val, intensity);
            if(color == COLOR_LINE_BLUE)
                mask1 = isBlueLine(r_val, g_val, b_val, intensity);
            if(color == COLOR_LINE_PURPLE)
                mask1 = isPurpleLine(r_val, g_val, b_val, intensity);

            if(mask1==1) {
                uchar twofiftyfive = 255;
                result.at<uchar>(i,j) = twofiftyfive; // lol, because i'm not sure it works without doing this
            }

        }
    }

    return result;

}


double getIntensity(int r, int g, int b) {
    return 0.2126*r+0.7152*g+0.0722*b;
}

bool isRedBlock(int r, int g, int b, double intensity) {
    return (r > 1.3*g && r > 1.3*b
            /*&& r < 2.5*intensity+40*/ && (r > 1.4*intensity-15 || intensity > 230 && r > 250)
            && g < 0.9*intensity+10 && b < 0.9*intensity+10);
}

bool isGreenBlock(int r, int g, int b, double intensity) {
    return (g > 0.9*r && g > 0.9*b
            && (fabs(g-1.08*intensity-1.57) < 30 || ((intensity >200) && (255-g) < 30)) /*&& fabs(g-0.91*intensity-6) < 50*/
            && r < 0.95*intensity+30 && b < 0.95*intensity+30
            //&& isNotFloor(r,g,b,intensity)
            && !isBlueLine(r,g,b,intensity)
            && !isPurpleLine(r,g,b,intensity)
            && !isRedBlock(r,g,b,intensity));
}

bool isBlueLine(int r, int g, int b, double intensity) {
    return (b > 1.05*g && b > 1.05*r
            && fabs(g-1.07*intensity) < 30
            && !isPurpleLine(r,g,b,intensity));
}

bool isPurpleLine(int r, int g, int b, double intensity) {
    return (r > g && b > g
            && (fabs(g-(1*intensity-10)) < 30)
            && !isFloor(r,g,b,intensity)
            /*b > 1.05*g && b > 1.05*r
            && r > g
            && fabs(g-1.07*intensity) < 30*/
            );
}

bool isFloor(int r, int g, int b, double intensity) {
    return (r < 1.25*intensity && r > 0.75*intensity
             && g < 1.25*intensity && g > 0.75*intensity
             && b < 1.25*intensity && b > 0.75*intensity);
}



// takes in colors array to account or different combinations of colors
// e.g. blue & white --> wall / red & green --> stacks of blocks
cv::Mat detectColor(cv::Mat& frame, int colors[], int size_of_colors){

    cv::Mat result;

    // temp color detection
    for(int i = 0; i < size_of_colors; i++) {
        cv::Mat temp = detectColor(frame, colors[i]);
        result = cv::max(result, temp);
    }
    return result;

}






}
