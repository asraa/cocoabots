
#include "ImageUtils.h"


namespace ImageUtils{

void binaryImagePreProcess(cv::Mat& im_src){
  // smooth the image
  cv::blur(im_src, im_src, cv::Size(SMOOTH_KERNEL,SMOOTH_KERNEL));
  // remove random bits
  cv::Mat clean_kernel = cv::Mat::ones(MORPH_KERNEL, MORPH_KERNEL, CV_8U);
  cv::morphologyEx(im_src, im_src, cv::MORPH_OPEN, clean_kernel);
}

cv::Mat cannyEdge(cv::Mat& im_src){
  cv::Mat im_result;
  cv::Canny(im_src, im_result, CANNY_THRESH_LOW, CANNY_THRESH_UP, CANNY_KERNEL); // returns gray
  return im_result;
}



}
