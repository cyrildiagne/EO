#ifndef Body_h
#define Body_h

#include "Line.h"
#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Flat.h>
#include <memory>

using namespace Magnum;

class Body : public Line {
public:
  void setup(int numSegments) {
    std::vector<Vector2> pts;
    const float PI = 3.14159265359;
    for (int i = 0; i < numSegments + 1; i++) {
      float pct = static_cast<float>(i) / numSegments;
      pts.push_back(Vector2(cos(pct * PI * 2), sin(pct * PI * 2)));
    }
    setPoints(pts);
  }
};

#endif /* end of include guard: Body_h */
