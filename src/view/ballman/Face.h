#ifndef view_ballman_Face_h
#define view_ballman_Face_h

#include <memory>

#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Flat.h>

#include "view/primitives/Circle.h"
#include "view/primitives/Line.h"

using namespace Magnum;

class Face {
public:
  void setup();
  void update(Vector2 p, Vector2 s);
  void draw();

private:
  std::unique_ptr<Line> leftEye;
  std::unique_ptr<Line> rightEye;
  std::unique_ptr<Line> mouth;
};

#endif /* end of include guard: view_ballman_Face_h */
