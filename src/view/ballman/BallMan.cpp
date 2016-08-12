#include "view/ballman/BallMan.h"

#include <Magnum/Math/Vector.h>

using namespace Magnum;

void BallMan::setup() {
  // setup body
  body.setup(100, 64);
  // setup arms & legs
  leftArm.setup(20);
  rightArm.setup(20);
  leftLeg.setup();
  rightLeg.setup();
}

void BallMan::update(Vector2 p, float radius, float t) {
  float r = body.radius;
  Vector2 s = Vector2{radius / r, radius / r};
  // update arms
  leftArm.update(Vector2(p.x() - radius, p.y()), t);
  leftArm.scale = s;
  rightArm.update(Vector2(p.x() + radius, p.y()), t);
  rightArm.scale = s;
  // update legs
  Vector2 legsOffset =
      Matrix3::rotation(60.0_degf).transformVector({radius, 0});
  leftLeg.update(p + legsOffset, t);
  leftLeg.scale = s;
  Vector2 rightLegOffset{p.x() - legsOffset.x(), p.y() + legsOffset.y()};
  rightLeg.update(rightLegOffset, t);
  rightLeg.scale = s;
  // update body radius
  body.scale = s;
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
