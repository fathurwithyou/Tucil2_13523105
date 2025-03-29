#include "QuadtreeNode.hpp"

QuadtreeNode::QuadtreeNode(int _x, int _y, int _width, int _height)
    : x(_x), y(_y), width(_width), height(_height), isLeaf(true) {
  for (int i = 0; i < 4; i++) {
    children[i] = nullptr;
  }
}

QuadtreeNode::~QuadtreeNode() {
  for (int i = 0; i < 4; i++) {
    if (children[i]) {
      delete children[i];
      children[i] = nullptr;
    }
  }
}
