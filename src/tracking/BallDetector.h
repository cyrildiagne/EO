#ifndef tracking_BallDetector_h
#define tracking_BallDetector_h

#include <opencv2/core/core.hpp>

namespace eo {
namespace tracking {

struct DetectedCircle {
  DetectedCircle(float x, float y, float radius) : x(x), y(y), radius(radius) {}
  float x;
  float y;
  float radius;
};

class BallDetector {
public:
  BallDetector();
  // detect
  void process(const cv::Mat &frame);
  // detected circles
  std::vector<DetectedCircle> circles;
  // intermediate mats
  cv::Mat resizedFrame, hsvFrame, threshFrame, morphFrame;
  // rescale scales the input frame before processing to speed up
  float rescale;
  // min and max HSV used for thresholding the input frame
  int minHue, maxHue;
  int minSaturation, maxSaturation;
  int minValue, maxValue;
};

} // namespace eo
} // namespace tracking

#endif /* end of include guard: tracking_BallDetector_h */
