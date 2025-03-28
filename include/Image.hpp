#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <vector>

#include "Color.hpp"
#include "QuadtreeNode.hpp"

class Image {
 public:
  Image(const std::vector<std::vector<Color>>& data, double thresh);
  ~Image();
  void printQuadtree() const;

 private:
  std::vector<std::vector<Color>> pixelData;
  QuadtreeNode* root;
  double threshold;
  QuadtreeNode* buildQuadtree(int x, int y, int width, int height);
};

#endif
