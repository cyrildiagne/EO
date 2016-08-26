#include "view/fx/Clap.h"

namespace eo {
namespace view {

void Clap::setup() {
  const float PI = 3.14159265359;
  const float radius1 = 50;
  const float radius2 = 80;
  const int num = 8;
  for (int i = 0; i < num; i++) {
    float theta = static_cast<float>(i) / num * PI * 2;
    std::vector<Vector2> pts;
    pts.push_back(Vector2(cos(theta) * radius1, sin(theta) * radius1));
    pts.push_back(Vector2(cos(theta) * radius2, sin(theta) * radius2));
    auto line = std::shared_ptr<Line>(new Line(pts));
    lines.push_back(line);
  }
}

void Clap::reset(Vector2 p) {
  scale.x() = scale.y() = 0.1;
  visible = true;
  for (auto &line : lines) {
    line->position = p;
    line->scale = scale;
    line->thickness = 25.f;
  }
}

void Clap::update() {
  if (lines[0]->scale.x() > 1.8f) {
    visible = false;
    return;
  }
  const Vector2 dest{2.f, 2.f};
  for (auto &line : lines) {
    line->scale += (dest - line->scale) * 0.175;
    line->thickness += (2.f - line->thickness) * 0.175;
  }
}

void Clap::draw() {
  if (!visible) {
    return;
  }
  for (auto &line : lines) {
    line->draw();
  }
}

void Clap::setColor(Magnum::Color3 color) {
  for (auto &line : lines) {
    line->color = color;
  }
}

} // namespace view
} // namespace eo
