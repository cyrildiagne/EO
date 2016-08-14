#ifndef tracking_BallFollower_h
#define tracking_BallFollower_h

#include "tracking/BallDetector.h"

namespace eo {
namespace tracking {

struct FollowedCircle {
  FollowedCircle(std::string label, DetectedCircle circle)
      : label(label), circle{circle}, numUpdatesMissing(0) {}
  std::string label;
  DetectedCircle circle;
  // velocity can be used to increase the matching accuracy and is applied on
  // items that have been missing for a few frames
  std::vector<float> velocity;
  // tracks the number of frame that this circle has been missing
  int numUpdatesMissing;
};

class BallFollower {
public:
  BallFollower() : currLabel(0){};
  void process(const std::vector<DetectedCircle> &detected);
  std::vector<FollowedCircle> circles;
  long int currLabel;
};

} // namespace eo
} // namespace tracking

#endif /* end of include guard: tracking_BallFollower_h */
