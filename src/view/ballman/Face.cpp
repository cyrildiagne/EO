#include "view/ballman/Face.h"

#include "view/primitives/Arc.h"
#include "view/primitives/Circle.h"
#include <iostream>

namespace eo {
namespace view {

void Face::setup() { applyNewExpression(); }

void Face::setExpression(Expression expr) { newExpression = expr; }

void Face::applyNewExpression() {
  if (currentExpression == newExpression) {
    return;
  }
  const float PI = 3.14159265359;
  switch (newExpression) {
  case Happy:
  case Extatic:
    leftEye = std::shared_ptr<Line>(new Arc(22, PI, PI));
    rightEye = std::shared_ptr<Line>(new Arc(22, PI, PI));
    mouth = std::shared_ptr<Line>(new Circle(1, 20));
    break;
  case Neutral:
  default:
    leftEye = std::shared_ptr<Line>(new Circle(8, 20));
    rightEye = std::shared_ptr<Line>(new Circle(8, 20));
    mouth = std::shared_ptr<Line>(new Circle(1, 20));
    break;
  }
  currentExpression = newExpression;
}

void Face::update(Vector2 p, Vector2 s) {
  // place eyes at 80% of face radius
  float r = 100.f * 0.8;
  // update body radius
  float offset = r * s.x() * 0.5;
  // left
  leftEye->scale = s;
  leftEye->position = p + Vector2{-offset, -offset * 0.2f};
  // right
  rightEye->scale = s;
  rightEye->position = p + Vector2{offset, -offset * 0.2f};
  // mouth
  mouth->scale = s;
  mouth->position = p + Vector2{0.f, offset};
}

void Face::draw() {
  leftEye->draw();
  rightEye->draw();
  // mouth->draw();
  // apply new expression after drawing to make sure new face gets update data
  applyNewExpression();
}

void Face::setColor(Magnum::Color4 color) {
  leftEye->color = color;
  rightEye->color = color;
  mouth->color = color;
}

} // namespace view
} // namespace eo
