#ifndef BallMan_h
#define BallMan_h

#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Trade/MeshData2D.h>

#include "BallManShader.h"
#include "Leg.h"

using namespace Magnum;

class BallMan {
public:
  void setup();
  void update(Vector2 p, float radius, float t);
  void draw();

private:
  Mesh mesh;
  std::unique_ptr<Buffer> vertices, indices;
  BallManShader shader;
  Matrix3 transform;
  Leg leftArm;
  Leg rightArm;
  Leg leftLeg;
  Leg rightLeg;
};

#endif /* end of include guard: BallMan_h */
