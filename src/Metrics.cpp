#include "Metrics.hpp"

double VarianceMetric::compute(const std::vector<std::vector<Color>>& pixels,
                               int x, int y, int width, int height) {
  long sumR = 0, sumG = 0, sumB = 0;
  int count = width * height;
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      sumR += pixels[i][j].r;
      sumG += pixels[i][j].g;
      sumB += pixels[i][j].b;
    }
  }
  double avgR = sumR / (double)count;
  double avgG = sumG / (double)count;
  double avgB = sumB / (double)count;
  double variance = 0.0;
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      variance += (pixels[i][j].r - avgR) * (pixels[i][j].r - avgR) +
                  (pixels[i][j].g - avgG) * (pixels[i][j].g - avgG) +
                  (pixels[i][j].b - avgB) * (pixels[i][j].b - avgB);
    }
  }
  return variance / count;
}

double MADMetric::compute(const std::vector<std::vector<Color>>& pixels, int x,
                          int y, int width, int height) {
  long sumR = 0, sumG = 0, sumB = 0;
  int count = width * height;
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      sumR += pixels[i][j].r;
      sumG += pixels[i][j].g;
      sumB += pixels[i][j].b;
    }
  }
  double avgR = sumR / (double)count;
  double avgG = sumG / (double)count;
  double avgB = sumB / (double)count;
  double mad = 0.0;
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      mad += std::abs(pixels[i][j].r - avgR) + std::abs(pixels[i][j].g - avgG) +
             std::abs(pixels[i][j].b - avgB);
    }
  }
  return mad / count;
}

double MaxPixelDifferenceMetric::compute(
    const std::vector<std::vector<Color>>& pixels, int x, int y, int width,
    int height) {
  int minR = 255, minG = 255, minB = 255;
  int maxR = 0, maxG = 0, maxB = 0;
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      const Color& c = pixels[i][j];
      minR = std::min(minR, c.r);
      minG = std::min(minG, c.g);
      minB = std::min(minB, c.b);
      maxR = std::max(maxR, c.r);
      maxG = std::max(maxG, c.g);
      maxB = std::max(maxB, c.b);
    }
  }
  double dR = maxR - minR;
  double dG = maxG - minG;
  double dB = maxB - minB;
  return (dR + dG + dB) / 3.0;
}

double EntropyMetric::compute(const std::vector<std::vector<Color>>& pixels,
                              int x, int y, int width, int height) {
  int count = width * height;
  std::vector<int> histR(256, 0), histG(256, 0), histB(256, 0);
  for (int i = y; i < y + height; i++) {
    for (int j = x; j < x + width; j++) {
      const Color& c = pixels[i][j];
      histR[c.r]++;
      histG[c.g]++;
      histB[c.b]++;
    }
  }
  double entropyR = 0.0, entropyG = 0.0, entropyB = 0.0;
  for (int i = 0; i < 256; i++) {
    if (histR[i] > 0) {
      double p = histR[i] / (double)count;
      entropyR -= p * std::log2(p);
    }
    if (histG[i] > 0) {
      double p = histG[i] / (double)count;
      entropyG -= p * std::log2(p);
    }
    if (histB[i] > 0) {
      double p = histB[i] / (double)count;
      entropyB -= p * std::log2(p);
    }
  }
  return (entropyR + entropyG + entropyB) / 3.0;
}
