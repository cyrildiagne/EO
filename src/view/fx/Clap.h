#ifndef view_ballman_Clap_h
#define view_ballman_Clap_h

#include <memory>

#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Flat.h>

#include "view/primitives/Circle.h"
#include "view/primitives/Line.h"

using namespace Magnum;

namespace eo {
namespace view {

class Clap {
public:
  Clap() : visible(true), scale{1.f, 1.f} {}
  void setup();
  void reset(Vector2 p);
  void update();
  void draw();

  void setColor(Magnum::Color3 color);
  bool visible;

private:
  std::vector<std::shared_ptr<Line>> lines;
  Vector2 scale;
};

} // namespace view
} // namespace eo

#endif /* end of include guard: view_ballman_Clap_h */
