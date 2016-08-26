#include "view/ballman/Leg.h"

namespace eo {
namespace view {

Leg::Leg()
    : targetLeg(nullptr), isVisible(true), gravity(0.0), segmentLength(5) {}

void Leg::setup(Vector2 pos, int numSegments) {
  pts.push_back(VerletPoint(pos, true));
  for (int i = 0; i < numSegments; i++) {
    pts.push_back(VerletPoint(pos));
  }
  // hand.setup(1, 32);
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
    vpts[i].x() = pts[i].x() / scale.x();
    vpts[i].y() = pts[i].y() / scale.y();
  }
  setPoints(vpts);
  // update hand
  // hand.thickness = thickness - 2;
  // hand.position = pts[0];
  // hand.color = color;
}

// void Leg::draw() {
//   hand.scale = scale;
//   hand.draw();
//   Line::draw();
// }

void Leg::applyUnitaryVerletIntegration(VerletPoint &p, float t) {
  p.x() = 2 * p.x() - p.prev.x();
  p.y() = 2 * p.y() - p.prev.y() + gravity * t;
}

void Leg::applyUnitaryDistanceRelaxation(VerletPoint &p,
                                         const VerletPoint &from) {
  Vector2 d{p.x() - from.x(), p.y() - from.y()};
  float dstFrom = d.length();
  if (dstFrom > segmentLength * scale.x() && dstFrom != 0) {
    p -= (dstFrom - segmentLength * scale.x()) * (d / dstFrom) * 0.5;
  }
}

} // namespace view
} // namespace eo
