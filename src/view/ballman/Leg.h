#ifndef Leg_h
#define Leg_h

#include <memory>

#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Flat.h>

#include "view/primitives/Line.h"

using namespace Magnum;

namespace eo {
namespace view {

class VerletPoint : public Vector2 {
public:
  VerletPoint(Vector2 pos, bool isPinned = false)
      : Vector2(pos), prev(pos), isPinned(isPinned) {}
  Vector2 prev;
  bool isPinned;
};

class Leg : public Line {
public:
  Leg();
  void setup(Vector2 pos, int numSegments = 15);
  void update(Vector2 pos, float t);

  std::vector<VerletPoint> pts;
  Leg *targetLeg;

private:
  Vector2 origin;
  float gravity;
  float segmentLength;

  void applyUnitaryVerletIntegration(VerletPoint &p, float t);
  void applyUnitaryDistanceRelaxation(VerletPoint &p, const VerletPoint &from);
};

} // namespace view
} // namespace eo

#endif /* end of include guard: Leg_h */
