#ifndef view_ballman_Contours_h
#define view_ballman_Contours_h

#include "tracking/BallDetector.h"
#include "view/primitives/Line.h"
#include <Magnum/DefaultFramebuffer.h>
#include <iostream>

using namespace Magnum;

namespace eo {
namespace view {

class Contours : public Line {
public:
  Contours() {}
  // setup
  void setup(const tracking::DetectedCircle::Blob &blob) { update(blob); }

  void update(const tracking::DetectedCircle::Blob &blob) {
    Vector2i size = defaultFramebuffer.viewport().size();
    float screenScale = static_cast<float>(size.x()) / 1280;
    std::vector<Vector2> pts;
    for (size_t i = 0; i < blob.size(); i++) {
      pts.push_back(Vector2(blob[i][0] / scale.x(), blob[i][1] / scale.y()) *
                    screenScale);
    }
    pts.push_back(Vector2(blob[0][0] / scale.x(), blob[0][1] / scale.y()) *
                  screenScale);
    setPoints(pts);
  }
};

} // namespace view
} // namespace eo

#endif /* end of include guard: view_ballman_Contours_h */
