#include "view/ballman/Face.h"

namespace eo {
namespace view {

void Face::setup() {
  leftEye = std::unique_ptr<Line>(new Circle(8, 20));
  rightEye = std::unique_ptr<Line>(new Circle(8, 20));
  mouth = std::unique_ptr<Line>(new Circle(1, 20));
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
  mouth->draw();
}

void Face::setColor(Magnum::Color3 color) {
  leftEye->color = color;
  rightEye->color = color;
  mouth->color = color;
}

} // namespace view
} // namespace eo
