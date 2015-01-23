
#include "GridMap.h"
#include "ImageProcessor.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
  ImageProcessor image_process;
  while(1) {
    std::cout << "found cube" << image_process.getFoundCube() << std::endl;
    usleep(100000);
  }
}
