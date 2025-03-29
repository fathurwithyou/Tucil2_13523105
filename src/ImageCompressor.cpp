#include "ImageCompressor.hpp"

double getFileSizeInMB(const std::string& filename) {
  struct stat st;
  if (stat(filename.c_str(), &st) != 0) return -1;
  return st.st_size / (1024.0 * 1024.0);
}

// Load image from the input path and populate pixelData.
void ImageCompressor::loadImageFromPath() {
  FreeImage_Initialise();
  FIBITMAP* bitmap =
      FreeImage_Load(FreeImage_GetFileType(inputImagePath.c_str(), 0),
                     inputImagePath.c_str(), JPEG_DEFAULT);
  if (!bitmap) {
    std::cerr << "Error: Cannot load image from " << inputImagePath
              << std::endl;
    exit(1);
  }
  FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
  int width = FreeImage_GetWidth(bitmap32);
  int height = FreeImage_GetHeight(bitmap32);
  if (width == 0 || height == 0) {
    std::cerr << "Error: Invalid image dimensions!" << std::endl;
    exit(1);
  }
  pixelData.resize(height, std::vector<Color>(width));

  BYTE* bits = FreeImage_GetBits(bitmap32);
  unsigned int pitch = FreeImage_GetPitch(bitmap32);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int index = i * pitch + j * 4;
      pixelData[i][j].r = bits[index + 2];
      pixelData[i][j].g = bits[index + 1];
      pixelData[i][j].b = bits[index + 0];
    }
  }
  FreeImage_Unload(bitmap32);
  FreeImage_Unload(bitmap);
  FreeImage_DeInitialise();
}

// Get user input for paths, error method, threshold, block size, and target
// compression.
void ImageCompressor::getInput() {
  std::cout << "Enter the absolute path of the image to compress: ";
  std::getline(std::cin, inputImagePath);
  if (inputImagePath.empty()) {
    std::cerr << "Error: Image path cannot be empty!\n";
    exit(1);
  }

  std::cout << "Choose error calculation method (1: Variance, 2: MAD, 3: Max "
               "Pixel Difference, 4: Entropy, 5: SSIM): ";
  if (!(std::cin >> errorMethod) || (errorMethod < 1 || errorMethod > 5)) {
    std::cerr << "Error: Invalid error method!\n";
    exit(1);
  }

  std::cout << "Enter threshold value: ";
  if (!(std::cin >> threshold) || threshold < 0) {
    std::cerr << "Error: Threshold must be non-negative!\n";
    exit(1);
  }

  std::cout << "Enter minimum block size: ";
  if (!(std::cin >> minBlockSize) || minBlockSize <= 0) {
    std::cerr << "Error: Minimum block size must be greater than 0!\n";
    exit(1);
  }

  std::cout << "Enter target compression ratio (1.0 = 100%, 0 to disable): ";
  if (!(std::cin >> targetCompression) || targetCompression < 0 ||
      targetCompression > 1.0) {
    std::cerr << "Error: Target compression must be between 0 and 1.0!\n";
    exit(1);
  }

  std::cin.ignore();
  std::cout << "Enter the absolute path for the compressed image: ";
  std::getline(std::cin, outputImagePath);
  if (outputImagePath.empty()) {
    std::cerr << "Error: Output path cannot be empty!\n";
    exit(1);
  }

  std::cout << "Enter the absolute path for the GIF file (optional): ";
  std::getline(std::cin, gifPath);
}

// Process image: load image, build quadtree, and measure execution time.
void ImageCompressor::processImage() {
  loadImageFromPath();

  auto start = std::chrono::high_resolution_clock::now();

  quadtree = new Quadtree(pixelData, threshold, getMetric(), minBlockSize);

  auto end = std::chrono::high_resolution_clock::now();
  execTime = end - start;
}

// Return a new Metric instance based on the chosen error method.
Metric* ImageCompressor::getMetric() {
  switch (errorMethod) {
    case 1:
      return new VarianceMetric();
    case 2:
      return new MADMetric();
    case 3:
      return new MaxPixelDifferenceMetric();
    case 4:
      return new EntropyMetric();
    case 5:
      return new SSIMetric(threshold);
    default:
      return new VarianceMetric();
  }
}

// Save the compressed image to the output path.
void ImageCompressor::saveImage() {
  int width = pixelData[0].size();
  int height = pixelData.size();
  bool drawOutline = false;

  FreeImage_Initialise();
  FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
  if (!bitmap) {
    std::cerr << "Error: Cannot allocate bitmap." << std::endl;
    FreeImage_DeInitialise();
    return;
  }

  std::function<void(QuadtreeNode*)> drawNode = [&](QuadtreeNode* node) {
    if (node->isLeaf) {
      for (int y = node->y; y < node->y + node->height; y++) {
        for (int x = node->x; x < node->x + node->width; x++) {
          RGBQUAD col;
          col.rgbRed = node->color.r;
          col.rgbGreen = node->color.g;
          col.rgbBlue = node->color.b;
          FreeImage_SetPixelColor(bitmap, x, y, &col);
        }
      }
      if (drawOutline) {
        RGBQUAD outline = {0, 0, 0, 0};
        for (int x = node->x; x < node->x + node->width; x++)
          FreeImage_SetPixelColor(bitmap, x, node->y, &outline);
        for (int x = node->x; x < node->x + node->width; x++)
          FreeImage_SetPixelColor(bitmap, x, node->y + node->height - 1,
                                  &outline);
        for (int y = node->y; y < node->y + node->height; y++)
          FreeImage_SetPixelColor(bitmap, node->x, y, &outline);
        for (int y = node->y; y < node->y + node->height; y++)
          FreeImage_SetPixelColor(bitmap, node->x + node->width - 1, y,
                                  &outline);
      }
    } else {
      for (int i = 0; i < 4; i++) {
        if (node->children[i] != nullptr) drawNode(node->children[i]);
      }
    }
  };

  drawNode(quadtree->getRoot());

  if (!FreeImage_Save(FIF_JPEG, bitmap, outputImagePath.c_str(),
                      JPEG_QUALITYGOOD)) {
    std::cerr << "Error: Failed to save the image as JPG." << std::endl;
  } else {
    std::cout << "[OUTPUT] Compressed image saved at: " << outputImagePath
              << std::endl;
  }
  FreeImage_Unload(bitmap);
  FreeImage_DeInitialise();
}

// Save a GIF animation of the compression process using ImageMagick.
void ImageCompressor::saveGif() {
  int maxDepth = quadtree->getTreeDepth();
  std::vector<std::string> frameFilenames;
  auto isFileExist = [&](const std::string& filename) -> bool {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
  };

  for (int depth = 0; depth <= maxDepth; depth++) {
    FIBITMAP* frameBitmap = quadtree->createImage(depth, true);
    if (!frameBitmap) {
      std::cerr << "Error: Failed to create image for depth " << depth
                << std::endl;
      continue;
    }
    std::ostringstream oss;
    oss << "frame_" << depth << ".png";
    std::string frameFile = oss.str();
    frameFilenames.push_back(frameFile);

    if (!FreeImage_Save(FIF_PNG, frameBitmap, frameFile.c_str(), 0)) {
      std::cerr << "Error: Failed to save frame " << depth << std::endl;
    }
    FreeImage_Unload(frameBitmap);
  }

  std::ostringstream cmd;
  cmd << "convert -delay 50 -loop 0";
  for (const auto& frame : frameFilenames) {
    if (isFileExist(frame))
      cmd << " " << frame;
    else
      std::cerr << "Warning: Frame file " << frame << " does not exist."
                << std::endl;
  }
  cmd << " " << gifPath;

  int ret = system(cmd.str().c_str());
  if (ret != 0) {
    std::cerr << "Error: Failed to create GIF using ImageMagick. Command: "
              << cmd.str() << std::endl;
  }

  for (const auto& frame : frameFilenames) {
    if (isFileExist(frame)) {
      if (std::remove(frame.c_str()) != 0)
        std::cerr << "Error: Failed to remove temporary frame " << frame
                  << std::endl;
    }
  }
  std::cout << "[OUTPUT] GIF saved at: " << gifPath << std::endl;
}

void ImageCompressor::showStats() {
  printf("----------------- [ STATS ] -----------------\n");
  printf("[INFO] Max Depth: %d\n", quadtree->getTreeDepth());
  printf("[INFO] Execution Time: %.2f sec\n", execTime.count());
  printf("[INFO] Original File Size: %.2f MB\n",
         getFileSizeInMB(inputImagePath));
  printf("[INFO] Compressed File Size: %.2f MB\n",
         getFileSizeInMB(outputImagePath));

  double compressPercentage =
      (1 - getFileSizeInMB(outputImagePath) / getFileSizeInMB(inputImagePath)) *
      100;
  printf("[INFO] Compression Percentage: %.2f%%\n", compressPercentage);
}

// Run the complete image compression process.
void ImageCompressor::run() {
  getInput();
  processImage();
  showStats();
  saveImage();
  saveGif();
  delete quadtree;
}
