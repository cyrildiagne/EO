#ifndef Leg_h
#define Leg_h

#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Flat.h>
#include <memory>

using namespace Magnum;

class VerletPoint : public Vector2 {
public:
  VerletPoint(bool isPinned = false) : isPinned(isPinned) {}
  Vector2 prev;
  bool isPinned;
};

class Leg {
public:
  Leg();
  void setup(int numSegments = 15);
  void update(Vector2 pos, float t);
  void draw();

private:
  Vector2 origin;
  std::vector<VerletPoint> pts;
  float gravity;
  float segmentLength;

  void applyUnitaryVerletIntegration(VerletPoint &p, float t);
  void applyUnitaryDistanceRelaxation(VerletPoint &p, const VerletPoint &from);

  Shaders::Flat2D shader;
  Buffer buffer;
  Mesh mesh;
};

#endif /* end of include guard: Leg_h */
