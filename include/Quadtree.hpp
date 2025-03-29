#ifndef __QUADTREE_HPP__
#define __QUADTREE_HPP__

#include <FreeImage.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "Color.hpp"
#include "Metrics.hpp"
#include "QuadtreeNode.hpp"

class Quadtree {
 private:
  std::vector<std::vector<Color>> pixelData;
  QuadtreeNode* root;
  double threshold;
  int minBlockSize;
  Metric* metric;
  QuadtreeNode* buildQuadtree(int x, int y, int width, int height,
                              int depth = 0);
  Color calculateAverageColor(const std::vector<std::vector<Color>>& data,
                              int x, int y, int width, int height);

 public:
  Quadtree(const std::vector<std::vector<Color>>& data, double thresh,
           Metric* metric, int minBlockSize);
  ~Quadtree();
  int getTreeDepth() const;
  int getNodeCount() const;
  int getLeafCount() const;
  FIBITMAP* createImage(int customDepth, bool showLines);
  QuadtreeNode* getRoot() const { return root; }
};

#endif
