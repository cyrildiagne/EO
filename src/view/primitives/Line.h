#ifndef Line_h
#define Line_h

#include <memory>

#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>

#include "view/primitives/LineShader.h"

using namespace Magnum;

class Line {
public:
  Line();
  ~Line();
  void setPoints(const std::vector<Vector2> &pts);
  void draw();

  float thickness;
  Color3 color;
  Vector2 scale;
  Vector2 position;

private:
  std::vector<Vector2> computeNormals(const std::vector<Vector2> &pts);
  Buffer buffer;
  Mesh mesh;
  LineShader shader;
};

#endif /* end of include guard: Line_h */
