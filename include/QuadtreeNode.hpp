#ifndef __QUADTREE_NODE_HPP__
#define __QUADTREE_NODE_HPP__

#include "Color.hpp"

class QuadtreeNode {
 public:
  int x, y, width, height, depth;
  bool isLeaf;
  Color color;
  QuadtreeNode* children[4];

  QuadtreeNode(int _x, int _y, int _width, int _height);
  ~QuadtreeNode();
};

#endif
