#include "Leg.h"

#include "MagnumUtils.h"

Leg::Leg() : gravity(0.0), segmentLength(5) {}

void Leg::setup(int numSegments) {
  pts.push_back(VerletPoint(true));
  for (int i = 0; i < numSegments; i++) {
    pts.push_back(VerletPoint());
  }
}

void Leg::update(Vector2 pos, float t) {
  origin = pos;
  pts[0].x() = origin.x();
  pts[0].y() = origin.y();
  // apply verlet integration
  Vector2 prev;
  for (auto &p : pts) {
    prev.x() = p.x();
    prev.y() = p.y();
    if (!p.isPinned)
      applyUnitaryVerletIntegration(p, t);
    p.prev.x() = prev.x();
    p.prev.y() = prev.y();
  }
  // apply relaxation
  int relaxIterations = 10;
  for (int r = 0; r < relaxIterations; r++) {
    for (size_t i = 1; i < pts.size(); i++) {
      VerletPoint &p = pts[i];
      if (!p.isPinned)
        applyUnitaryDistanceRelaxation(p, pts[i - 1]);
    }
    for (int i = pts.size() - 2; i > 0; i--) {
      VerletPoint &p = pts[i];
      if (!p.isPinned)
        applyUnitaryDistanceRelaxation(p, pts[i + 1]);
    }
  }
  // update Line points
  std::vector<Vector2> vpts(pts.size());
  for (size_t i = 0; i < pts.size(); i++) {
    vpts[i] = pts[i];
  }
  setPoints(vpts);
}

void Leg::applyUnitaryVerletIntegration(VerletPoint &p, float t) {
  p.x() = 2 * p.x() - p.prev.x();
  p.y() = 2 * p.y() - p.prev.y() + gravity * t;
}

void Leg::applyUnitaryDistanceRelaxation(VerletPoint &p,
                                         const VerletPoint &from) {
  Vector2 d{p.x() - from.x(), p.y() - from.y()};
  float dstFrom = d.length();
  if (dstFrom > segmentLength && dstFrom != 0) {
    p -= (dstFrom - segmentLength) * (d / dstFrom) * 0.5;
  }
}
