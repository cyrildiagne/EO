#ifndef view_primitives_Circle_h
#define view_primitives_Circle_h

#include <memory>

#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Flat.h>

#include "view/primitives/Line.h"

using namespace Magnum;

class Circle : public Line {
public:
  Circle() {}
  Circle(float radius, int numSegments) { setup(radius, numSegments); }
  // setup
  void setup(float radius, int numSegments) {
    this->radius = radius;
    std::vector<Vector2> pts;
    const float PI = 3.14159265359;
    for (int i = 0; i < numSegments + 1; i++) {
      float pct = static_cast<float>(i) / numSegments;
      pts.push_back(
          Vector2(cos(pct * PI * 2) * radius, sin(pct * PI * 2) * radius));
    }
    pts.push_back(
        Vector2(cos(1.02 * PI * 2) * radius, sin(1.02 * PI * 2) * radius));
    setPoints(pts);
  }
  float radius;
};

#endif /* end of include guard: view_primitives_Circle_h */
