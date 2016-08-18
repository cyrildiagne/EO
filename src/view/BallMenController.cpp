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
      Color3 color{utils::random(1.f), utils::random(1.f), utils::random(1.f)};
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
}

void BallMenController::draw() {
  for (auto &b : ballmen) {
    b.second->draw();
  }
}

} // namespace view
} // namespace eo
