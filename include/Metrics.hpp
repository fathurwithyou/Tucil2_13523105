#ifndef __METRICS_HPP__
#define __METRICS_HPP__

#include <algorithm>
#include <cmath>
#include <vector>

#include "Color.hpp"

class Metric {
 public:
  virtual double compute(const std::vector<std::vector<Color>>& pixels, int x,
                         int y, int width, int height) = 0;
  virtual ~Metric() {}
};

class VarianceMetric : public Metric {
 public:
  double compute(const std::vector<std::vector<Color>>& pixels, int x, int y,
                 int width, int height) override;
};

class MADMetric : public Metric {
 public:
  double compute(const std::vector<std::vector<Color>>& pixels, int x, int y,
                 int width, int height) override;
};

class MaxPixelDifferenceMetric : public Metric {
 public:
  double compute(const std::vector<std::vector<Color>>& pixels, int x, int y,
                 int width, int height) override;
};

class EntropyMetric : public Metric {
 public:
  double compute(const std::vector<std::vector<Color>>& pixels, int x, int y,
                 int width, int height) override;
};

class SSIMetric : public Metric {
  private:
   double threshold;
  public:
  SSIMetric(double threshold);
  double compute(const std::vector<std::vector<Color>>& pixels, int x, int y,
                 int width, int height) override;
};

#endif
