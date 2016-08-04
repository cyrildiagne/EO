#include "BallMan.h"

#include <Magnum/Math/Vector.h>

using namespace Magnum;

void BallMan::setup() {
  // setup body
  body.setup(100, 32);
  // setup arms & legs
  leftArm.setup(20);
  rightArm.setup(20);
  leftLeg.setup();
  rightLeg.setup();
}

void BallMan::update(Vector2 p, float radius, float t) {
  // update arms
  leftArm.update(Vector2(p.x() - radius, p.y()), t);
  rightArm.update(Vector2(p.x() + radius, p.y()), t);
  // update legs
  Vector2 legsOffset =
      Matrix3::rotation(60.0_degf).transformVector({radius, 0});
  leftLeg.update(p + legsOffset, t);
  Vector2 rightLegOffset{p.x() - legsOffset.x(), p.y() + legsOffset.y()};
  rightLeg.update(rightLegOffset, t);
  // update body radius
  body.position = p;
}

void BallMan::draw() {
  // draw body
  body.draw();
  // draw arms & legs
  leftArm.draw();
  rightArm.draw();
  leftLeg.draw();
  rightLeg.draw();
}
