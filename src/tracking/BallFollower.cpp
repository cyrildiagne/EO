#include "BallFollower.h"

#include <iostream>

namespace eo {
namespace tracking {

inline float GetFollowDistanceSquared(const FollowedCircle &a,
                                      const DetectedCircle &b) {
  const float dx = b.x - a.circle.x;
  const float dy = b.y - a.circle.y;
  const float dr = b.radius - a.circle.radius;
  return dx * dx + dy * dy + dr * dr;
}

// most of the logic here borrowed from ofxCv
// https://github.com/kylemcdonald/ofxCv
void BallFollower::process(const std::vector<DetectedCircle> &detected) {
  float maxDistance = 500000;
  std::vector<FollowedCircle> previous = circles;

  // build NxM distance matrix
  typedef std::pair<int, int> MatchPair;
  typedef std::pair<MatchPair, float> MatchDistancePair;
  std::vector<MatchDistancePair> all;
  const int n = detected.size();
  const int m = previous.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      const float d = GetFollowDistanceSquared(previous[j], detected[i]);
      if (d < maxDistance) {
        all.push_back({MatchPair{i, j}, d});
      }
    }
  }
  // sort all possible matches by distance
  sort(all.begin(), all.end(),
       [](const MatchDistancePair &a, const MatchDistancePair &b) {
         return a.second < b.second;
       });

  circles.clear();
  // match pairs
  std::vector<bool> matchedDetected(n, false);
  std::vector<bool> matchedPrevious(m, false);
  for (size_t k = 0; k < all.size(); k++) {
    const MatchPair &match = all[k].first;
    int i = match.first;
    int j = match.second;
    // if both items haven't been matched yet
    if (!matchedDetected[i] && !matchedPrevious[j]) {
      // lock further matching for these items
      matchedDetected[i] = true;
      matchedPrevious[j] = true;
      // add to circles vector
      previous[j].velocity = {
          detected[i].x - previous[j].circle.x,
          detected[i].y - previous[j].circle.y,
          detected[i].radius - previous[j].circle.radius,
      };
      previous[j].circle = detected[i];
      previous[j].numUpdatesMissing = 0;
      circles.push_back(previous[j]);
    }
  }

  // add circles that are missing
  for (size_t j = 0; j < previous.size(); j++) {
    if (!matchedPrevious[j]) {
      if (++previous[j].numUpdatesMissing < 30) {
        // TODO: compute velocity and apply to position+radius based on history
        // previous[j].circle.x += previous[j].velocity[0];
        // previous[j].circle.y += previous[j].velocity[1];
        // previous[j].circle.radius += previous[j].velocity[2];
        circles.push_back(previous[j]);
      }
    }
  }

  // create circles for detected that haven't been matched
  for (size_t i = 0; i < detected.size(); i++) {
    if (!matchedDetected[i]) {
      std::ostringstream label;
      label << currLabel;
      circles.push_back({label.str(), detected[i]});
      currLabel++;
    }
  }
}

} // namespace eo
} // namespace tracking
