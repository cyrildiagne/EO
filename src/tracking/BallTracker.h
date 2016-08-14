#ifndef tracking_BallTracker_h
#define tracking_BallTracker_h

#include <opencv2/core/core.hpp>

#include "tracking/BallDetector.h"
#include "tracking/BallFollower.h"

namespace eo {
namespace tracking {

class BallTracker {
public:
  enum FrameId { Input, Resize, HSV, Threshold, Morph };
  void setup();
  void update(const cv::Mat &frame);
  double getTrackTime() { return trackTime; }
  // returns the lastest frame of currFrameId
  const cv::Mat &getCurrFrame();
  void setNextFrameId();
  // detector finds blobs a cv frame
  BallDetector detector;
  // follower follows identities of the blobs
  BallFollower follower;

private:
  // latest processed frame depending on currFrameId
  cv::Mat currFrame;
  FrameId currFrameId;
  // store chrono result
  double trackTime;
};

} // namespace eo
} // namespace tracking

#endif /* end of include guard: tracking_BallTracker_h */
