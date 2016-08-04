#ifndef Line_h
#define Line_h

#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Flat.h>
#include <memory>

using namespace Magnum;
using namespace Magnum::Shaders;

class Line {
public:
  Line();
  void setPoints(const std::vector<Vector2> &pts);
  void draw();

  float thickness;
  Vector2 scale;
  Vector2 position;

private:
  std::vector<Vector2> getRibbon(const std::vector<Vector2> &pts);
  Buffer buffer;
  Mesh mesh;
  Flat2D shader;
};

#endif /* end of include guard: Line_h */
