#include "opencv2/opencv.hpp"
#include <string>
#include <sstream>

using namespace cv;


int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
     cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
     cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);    
    Mat edges;
    namedWindow("edges",1);
    int n = 1;
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        imshow("edges", frame);
        if(waitKey(30) >= 0) {
	  std::ostringstream filename;
	  filename << "images/blocks_" << n <<".jpg";
	  imwrite(filename.str(),frame);
	  n = n+1;
	}
	
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
