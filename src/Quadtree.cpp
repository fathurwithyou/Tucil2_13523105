#include "Quadtree.hpp"

// Create an image by coloring each leaf node with its average color.
FIBITMAP* Quadtree::createImage(int customDepth, bool showLines) {
  int width = pixelData[0].size();
  int height = pixelData.size();

  FreeImage_Initialise();
  FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
  if (!bitmap) {
    std::cerr << "Error: Cannot allocate bitmap in createImage()." << std::endl;
    FreeImage_DeInitialise();
    return nullptr;
  }

  // Recursively draw each node from the quadtree into the bitmap.
  std::function<void(QuadtreeNode*)> drawNode = [&](QuadtreeNode* node) {
    if (!node) return;
    if (node->isLeaf || node->depth >= customDepth) {
      for (int y = node->y; y < node->y + node->height; y++) {
        for (int x = node->x; x < node->x + node->width; x++) {
          RGBQUAD col;
          col.rgbRed = node->color.r;
          col.rgbGreen = node->color.g;
          col.rgbBlue = node->color.b;
          FreeImage_SetPixelColor(bitmap, x, y, &col);
        }
      }
    } else {
      for (int i = 0; i < 4; i++) {
        if (node->children[i] != nullptr) {
          drawNode(node->children[i]);
        }
      }
    }
  };

  drawNode(root);
  FreeImage_DeInitialise();
  return bitmap;
}

// Constructor: Build a quadtree from image data using the given threshold and metric.
Quadtree::Quadtree(const std::vector<std::vector<Color>>& data,
                   double threshold, Metric* metric, int minBlockSize)
    : pixelData(data),
      threshold(threshold),
      minBlockSize(minBlockSize),
      metric(metric) {
  int width = data[0].size();
  int height = data.size();
  root = buildQuadtree(0, 0, width, height);
}

// Destructor: Delete the root node, which recursively deletes the whole quadtree.
Quadtree::~Quadtree() { delete root; }

// Calculate the average color for the specified block of the image.
Color Quadtree::calculateAverageColor(const std::vector<std::vector<Color>>& data,
                                        int x, int y, int width, int height) {
  float r = 0, g = 0, b = 0;
  int count = 0;
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      r += data[i][j].r;
      g += data[i][j].g;
      b += data[i][j].b;
      count++;
    }
  }
  if (count == 0) return Color{0, 0, 0};
  int avgR = static_cast<int>(r / count);
  int avgG = static_cast<int>(g / count);
  int avgB = static_cast<int>(b / count);
  return Color{avgR, avgG, avgB};
}

// Recursively build the quadtree by subdividing blocks that exceed the threshold error.
QuadtreeNode* Quadtree::buildQuadtree(int x, int y, int width, int height, int depth) {
  float var = metric->compute(pixelData, x, y, width, height);
  QuadtreeNode* node = new QuadtreeNode(x, y, width, height);
  node->depth = depth;
  node->color = calculateAverageColor(pixelData, x, y, width, height);
  node->isLeaf = true;
  if (var >= threshold && (width / 2) * (height / 2) >= minBlockSize) {
    node->isLeaf = false;
    int halfWidth = width / 2;
    int halfHeight = height / 2;
    node->children[0] = buildQuadtree(x, y, halfWidth, halfHeight, depth + 1);
    node->children[1] = buildQuadtree(x + halfWidth, y, width - halfWidth, halfHeight, depth + 1);
    node->children[2] = buildQuadtree(x, y + halfHeight, halfWidth, height - halfHeight, depth + 1);
    node->children[3] = buildQuadtree(x + halfWidth, y + halfHeight, width - halfWidth, height - halfHeight, depth + 1);
  }
  return node;
}

// Get the maximum depth of the quadtree by recursively exploring each node.
int Quadtree::getTreeDepth() const {
  std::function<int(QuadtreeNode*)> depth = [&](QuadtreeNode* node) -> int {
    if (!node) return 0;
    if (node->isLeaf) return 1;
    int maxDepth = 0;
    for (int i = 0; i < 4; i++) {
      maxDepth = std::max(maxDepth, depth(node->children[i]));
    }
    return 1 + maxDepth;
  };
  return depth(root);
}

// Count the total number of nodes in the quadtree (internal + leaf nodes).
int Quadtree::getNodeCount() const {
  std::function<int(QuadtreeNode*)> countNodes = [&](QuadtreeNode* node) -> int {
    if (!node) return 0;
    int count = 1;
    if (!node->isLeaf) {
      for (int i = 0; i < 4; i++) {
        count += countNodes(node->children[i]);
      }
    }
    return count;
  };
  return countNodes(root);
}

// Count the number of leaf nodes in the quadtree.
int Quadtree::getLeafCount() const {
  std::function<int(QuadtreeNode*)> countLeaves = [&](QuadtreeNode* node) -> int {
    if (!node) return 0;
    if (node->isLeaf) return 1;
    int count = 0;
    for (int i = 0; i < 4; i++) {
      count += countLeaves(node->children[i]);
    }
    return count;
  };
  return countLeaves(root);
}
