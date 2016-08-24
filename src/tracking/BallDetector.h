#ifndef tracking_BallDetector_h
#define tracking_BallDetector_h

#include <opencv2/core/core.hpp>

namespace eo {
namespace tracking {

class DetectedCircle {
public:
  typedef std::vector<std::vector<float>> Blob;
  DetectedCircle(float x, float y, float radius, Blob blob, bool isCircle)
      : x(x), y(y), radius(radius), isCircle(isCircle), blob(blob) {}
  float x;
  float y;
  float radius;
  bool isCircle;
  Blob blob;
};

class BallDetector {
public:
  BallDetector();
  // detect
  void process(const cv::Mat &frame, bool bDebug);
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
