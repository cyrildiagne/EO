#ifndef BallMan_h
#define BallMan_h

#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Trade/MeshData2D.h>

#include "Leg.h"

using namespace Magnum;

class BallMan {
public:
  void setup() {
    std::tie(mesh, vertices, indices) = MeshTools::compile(
        Primitives::Circle::wireframe(16), BufferUsage::StaticDraw);
  }
  void update(double x, double y, double radius) {
    // retrieve viewport size
    Vector2i size = defaultFramebuffer.viewport().size();
    // update transformation
    transformation =
        Matrix3::translation(Vector2(x / size.x() * 2, -y / size.y() * 2)) *
        Matrix3::scaling(Vector2(radius / size.x(), radius / size.y()));
  }
  void draw() {
    // retrieve projection matrix
    Vector2i size = defaultFramebuffer.viewport().size();
    Matrix3 projection = Matrix3::scaling(Vector2::yScale(size.aspectRatio()));
    shader.setTransformationProjectionMatrix(projection * transformation)
        .setColor(Color3::red());
    // draw circle
    mesh.draw(shader);
  }

private:
  Mesh mesh;
  std::unique_ptr<Buffer> vertices, indices;
  Shaders::Flat2D shader;
  Matrix3 transformation;
};

#endif /* end of include guard: BallMan_h */
