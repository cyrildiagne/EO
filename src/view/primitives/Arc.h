#ifndef view_primitives_Arc_h
#define view_primitives_Arc_h

#include "view/primitives/Line.h"

using namespace Magnum;

namespace eo {
namespace view {

class Arc : public Line {
public:
  // setup
  Arc(float radius, float arc, float rotation) {
    std::vector<Vector2> pts;
    int numSegments = 16;
    for (int i = 0; i < numSegments + 1; i++) {
      float pct = static_cast<float>(i) / numSegments;
      pts.push_back(Vector2(cos(pct * arc + rotation) * radius,
                            sin(pct * arc + rotation) * radius));
    }
    setPoints(pts);
  }
};

} // namespace view
} // namespace eo

#endif /* end of include guard: view_primitives_Arc_h */
