#ifndef tracking_BallFollower_h
#define tracking_BallFollower_h

#include "tracking/BallDetector.h"

namespace eo {
namespace tracking {

class BallFollower {
public:
  BallFollower(){};
  void process(const std::vector<Circle> &circles) {
    if (circles.size()) {
    }
  }
};

} // namespace eo
} // namespace tracking

#endif /* end of include guard: tracking_BallFollower_h */
