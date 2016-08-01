#ifndef BallTracker_h
#define BallTracker_h

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

struct Circle {
  Circle(float x, float y, float radius) : x(x), y(y), radius(radius) {}
  float x;
  float y;
  float radius;
};

class BallTracker {
public:
  void setup();
  void update(const cv::Mat frame);
  std::vector<Circle> circles;
};
#endif /* end of include guard: BallTracker_h */
