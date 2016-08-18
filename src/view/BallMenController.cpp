#include "BallMenController.h"

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Math/Vector.h>

#include "utils.h"

using namespace Magnum;
using namespace eo::tracking;

namespace eo {
namespace view {

void BallMenController::update(const std::vector<FollowedCircle> &circles,
                               double dt) {
  Vector2i size = defaultFramebuffer.viewport().size();
  float screenScale = static_cast<float>(size.x()) / 1280;
  // save all the ballmen that have been updated here
  auto deadBallMen = ballmen;
  // loop through all tracked circles
  for (const tracking::FollowedCircle &c : circles) {
    auto ball = ballmen.find(c.label);
    // retrieve position & radius
    const Vector2 position = Vector2(c.circle.x, c.circle.y) * screenScale;
    const float radius = c.circle.radius * screenScale;
    // circle needs a new character
    if (ball == ballmen.end()) {
      ballmen[c.label] = std::shared_ptr<BallMan>(new BallMan);
      // Color3 color{utils::random(1.f), utils::random(1.f),
      // utils::random(1.f)};
      Color3 color{1.f, 1.f, utils::random(1.f)};
      ballmen[c.label]->setup(position, radius, color);
      ball = ballmen.find(c.label);
    } else {
      // otherwise remove the ball from the deadballs list
      auto it = deadBallMen.find(ball->first);
      if (it != deadBallMen.end()) {
        deadBallMen.erase(it);
      }
      // and sync visibility status
      ball->second->visible = (c.numUpdatesMissing == 0);
    }
    // update ball
    ball->second->update(position, radius, dt);
  }
  // remove characters that don't have circles anymore
  for (const auto &b : deadBallMen) {
    auto it = ballmen.find(b.first);
    if (it != ballmen.end()) {
      ballmen.erase(it);
    }
  }
  // update claps
  updateClaps();
}

void BallMenController::updateClaps() {
  // loop through all ball pairs
  for (auto it = ballmen.begin(); it != ballmen.end(); ++it) {
    // first check if character has leg attached
    auto &b1 = it->second;
    if (b1->leftArm.targetLeg) {
      updateLegTarget(b1->leftArm);
    }
    if (b1->rightArm.targetLeg) {
      updateLegTarget(b1->rightArm);
    }
    // look for new targets
    for (auto jt = ballmen.begin(); jt != ballmen.end(); ++jt) {
      // skip self
      if (it == jt) {
        continue;
      }
      auto &b2 = jt->second;
      checkMatch(b1->leftArm, b2->leftArm);
      checkMatch(b1->rightArm, b2->rightArm);
      checkMatch(b1->leftArm, b2->rightArm);
      checkMatch(b1->rightArm, b2->leftArm);
    }
  }
}

void BallMenController::checkMatch(Leg &l1, Leg &l2) {
  // do not match arms tha are already paired
  if (!l1.targetLeg && !l2.targetLeg) {
    // check arms distance
    if ((l1.pts[0] - l2.pts[0]).length() < 300) {
      l1.targetLeg = &l2;
      l2.targetLeg = &l1;
    }
  }
  updateLegTarget(l1);
}

bool BallMenController::updateLegTarget(Leg &leg) {
  if (!leg.targetLeg) {
    return false;
  }
  const Vector2 shoulder = leg.pts[0];
  const Vector2 targetShoulder = leg.targetLeg->pts[0];
  if ((shoulder - targetShoulder).length() > 300) {
    leg.targetLeg->targetLeg = nullptr;
    leg.targetLeg = nullptr;
    return false;
  }
  // stir respective hands toward target shoulder
  leg.pts.back().x() = leg.pts.back().prev.x() = targetShoulder.x();
  leg.pts.back().y() = leg.pts.back().prev.y() = targetShoulder.y();
  leg.targetLeg->pts.back().x() = leg.targetLeg->pts.back().prev.x() =
      shoulder.x();
  leg.targetLeg->pts.back().y() = leg.targetLeg->pts.back().prev.y() =
      shoulder.y();
  return true;
}

void BallMenController::draw() {
  for (auto &b : ballmen) {
    b.second->draw();
  }
}

} // namespace view
} // namespace eo
