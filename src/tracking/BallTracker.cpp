#include "tracking/BallTracker.h"
#include <chrono>

namespace eo {
namespace tracking {

void BallTracker::setup() { currFrameId = FrameId::Input; }

const cv::Mat &BallTracker::getCurrFrame() { return currFrame; }

void BallTracker::setNextFrameId() {
  currFrameId = static_cast<FrameId>(currFrameId + 1);
  if (currFrameId > FrameId::Morph) {
    currFrameId = FrameId::Input;
  }
}

void BallTracker::update(const cv::Mat &frame, bool bDebug) {
  // start chrono
  auto start = std::chrono::steady_clock::now();

  // detect blobs from input frame
  detector.process(frame, bDebug);
  // track identities
  follower.process(detector.circles);

  // update chrono
  auto end = std::chrono::steady_clock::now();
  auto diff = end - start;
  trackTime = std::chrono::duration<double, std::milli>(diff).count();

  // update currframe for rendering
  switch (currFrameId) {
  case FrameId::Input:
    currFrame = frame;
    break;
  case FrameId::Resize:
    currFrame = detector.resizedFrame;
    break;
  case FrameId::HSV:
    currFrame = detector.hsvFrame;
    break;
  case FrameId::Threshold:
    currFrame = detector.threshFrame;
    break;
  case FrameId::Morph:
    currFrame = detector.morphFrame;
    break;
  }
}
} // namespace tracking
} // namespace eo
