#include "BallFollower.h"

namespace eo {
namespace tracking {

void BallFollower::process(const std::vector<DetectedCircle> &detected) {
  circles.clear();
  int i = 0;
  for (const auto &c : detected) {
    std::ostringstream label;
    label << i;
    circles.push_back({label.str(), c});
    i++;
  }
}

} // namespace eo
} // namespace tracking
