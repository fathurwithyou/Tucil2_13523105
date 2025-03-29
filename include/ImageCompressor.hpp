#ifndef __IMAGECOMPRESSOR_HPP__
#define __IMAGECOMPRESSOR_HPP__

#include <FreeImage.h>
#include <sys/stat.h>

#include <chrono>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Metrics.hpp"
#include "Quadtree.hpp"

class ImageCompressor {
 private:
  std::string inputImagePath;
  int errorMethod;
  double threshold;
  int minBlockSize;
  std::chrono::duration<double> execTime;
  double targetCompression;
  std::string outputImagePath;
  std::string gifPath;
  Quadtree* quadtree;
  std::vector<std::vector<Color>> pixelData;

  void getInput();
  void processImage();
  void loadImage();
  void saveImage();
  void saveGif();
  void loadImageFromPath();
  void showStats();
  Metric* getMetric();

 public:
  void run();
};

#endif
