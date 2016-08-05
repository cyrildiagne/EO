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
  enum FrameId { Input, Resize, HSV, Threshold, Morph };
  void setup();
  void update(const cv::Mat frame);
  std::vector<Circle> circles;
  double getTrackTime() { return trackTime; }
  // returns the lastest frame of currFrameId
  const cv::Mat &getCurrFrame();
  void setNextFrameId();

private:
  // latest processed frame depending on currFrameId
  cv::Mat currFrame;
  FrameId currFrameId;
  // rescale scales the input frame before processing to speed up
  float rescale;
  // min and max HSV used for thresholding the input frame
  int minHue, maxHue;
  int minSaturation, maxSaturation;
  int minValue, maxValue;
  // store chrono result
  double trackTime;
};
#endif /* end of include guard: BallTracker_h */
