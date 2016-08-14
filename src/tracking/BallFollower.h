#ifndef tracking_BallFollower_h
#define tracking_BallFollower_h

#include "tracking/BallDetector.h"

namespace eo {
namespace tracking {

struct FollowedCircle {
  FollowedCircle(std::string label, DetectedCircle circle)
      : label(label), circle{circle} {}
  std::string label;
  DetectedCircle circle;
};

class BallFollower {
public:
  BallFollower(){};
  void process(const std::vector<DetectedCircle> &detected);
  std::vector<FollowedCircle> circles;
};

} // namespace eo
} // namespace tracking

#endif /* end of include guard: tracking_BallFollower_h */
