#ifndef Leg_h
#define Leg_h

#include <memory>

#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Flat.h>

#include "view/primitives/Line.h"

using namespace Magnum;

class VerletPoint : public Vector2 {
public:
  VerletPoint(bool isPinned = false) : isPinned(isPinned) {}
  Vector2 prev;
  bool isPinned;
};

class Leg : public Line {
public:
  Leg();
  void setup(int numSegments = 15);
  void update(Vector2 pos, float t);

private:
  Vector2 origin;
  std::vector<VerletPoint> pts;
  float gravity;
  float segmentLength;

  void applyUnitaryVerletIntegration(VerletPoint &p, float t);
  void applyUnitaryDistanceRelaxation(VerletPoint &p, const VerletPoint &from);
};

#endif /* end of include guard: Leg_h */
