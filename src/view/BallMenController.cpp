#include "BallMenController.h"

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Math/Vector.h>
#include <iostream>

#include "configure.h"
#include "utils.h"

using namespace Magnum;
using namespace eo::tracking;

namespace eo {
namespace view {

// BallMenController::~BallMenController() {}

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
      // ballmen[c.label]->yeye = std::make_shared<Magnum::Audio::Source>();
      // ballmen[c.label]->yeye->setLooping(false);
      // ballmen[c.label]->yeye->setGain(0.5);
      // ballmen[c.label]->yeye->setBuffer(
      //     &sound.clips[audio::Sound::Clip::YEYE].buffer);
      const Color3 color{0.85f, 0.95f, 0.95f};
      // Color3 color{0.15f, 0.2f, 0.15f};
      ballmen[c.label]->setup(position, radius, color, c.circle);
      ball = ballmen.find(c.label);
      // play sound effect
      // float rdm = utils::random(1.f);
      // if (rdm > 0.5) {
      //   sound.play(audio::Sound::NEW1);
      // } else {
      //   sound.play(audio::Sound::NEW2);
      // }
      sound.play(audio::Sound::CLAP2);
    } else {
      // otherwise remove the ball from the deadballs list
      const auto it = deadBallMen.find(ball->first);
      if (it != deadBallMen.end()) {
        deadBallMen.erase(it);
        // sound.play(audio::Sound::CLAP2);
      }
      // and sync visibility status
      ball->second->visible = (c.numUpdatesMissing == 0);
    }
    // update ball
    ball->second->update(position, radius, dt, c.circle);
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
    // set expression depending on the number of hands attached
    if (b1->leftArm.targetLeg || b1->rightArm.targetLeg) {
      if (b1->leftArm.targetLeg && b1->rightArm.targetLeg) {
        b1->face.setExpression(Face::Extatic);
      } else {
        b1->face.setExpression(Face::Happy);
      }
    } else {
      b1->face.setExpression(Face::Neutral);
    }
    b1->face.setColor(b1->leftArm.color);
  }
  // update visual clap
  clap.update();
  numUpdateSinceLastClap++;
}

void BallMenController::checkMatch(Leg &l1, Leg &l2) {
  // do not match arms tha are already paired
  if (!l1.targetLeg && !l2.targetLeg) {
    if (l1.isVisible && l2.isVisible) {
      // check arms distance
      if ((l1.pts[0] - l2.pts[0]).length() < 250) {
        l1.targetLeg = &l2;
        l2.targetLeg = &l1;
        // play sound
        playClap();
        // display visual feedback
        Vector2 center{(l1.pts[0] + l2.pts[0]) / 2};
        clap.reset(center);
      }
    }
  }
  updateLegTarget(l1);
}

bool BallMenController::updateLegTarget(Leg &leg) {
  if (!leg.targetLeg) {
    return false;
  }
  // separate the legs if one of them has become invisible
  if (leg.isVisible == false || leg.targetLeg->isVisible == false) {
    leg.targetLeg->targetLeg = nullptr;
    leg.targetLeg = nullptr;
    return false;
  }
  // check if distance should separate the legs
  const Vector2 shoulder = leg.pts[0];
  const Vector2 targetShoulder = leg.targetLeg->pts[0];
  if ((shoulder - targetShoulder).length() > 800) {
    leg.targetLeg->targetLeg = nullptr;
    leg.targetLeg = nullptr;
    sound.play(audio::Sound::CLAP3);
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
  clap.draw();
}

void BallMenController::setupClap() {
  // setup sound
  sound.setup();
  // setup visual element
  clap.setup();
  clap.setColor({0.85f, 0.95f, 0.95f});
}

void BallMenController::playClap() {
  // play sound
  if (numUpdateSinceLastClap > 5) {
    // float rdm = utils::random(1.f);
    // if (rdm > 0.5) {
    sound.play(audio::Sound::CLAP2);
    // } else if (rdm > 0.3) {
    // sound.play(audio::Sound::CLAP2);
    //} else {

    //}
    numUpdateSinceLastClap = 0;
  }
}

} // namespace view
} // namespace eo
