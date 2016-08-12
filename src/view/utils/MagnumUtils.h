#ifndef MagnumUtils_h
#define MagnumUtils_h

#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Trade/MeshData2D.h>

using namespace Magnum;

namespace MagnumUtils {

void drawDebugCircle(float x, float y, float radius = 50,
                     Color3 color = Color3::red()) {

  // retrieve viewport size
  Vector2i size = defaultFramebuffer.viewport().size();
  // retrieve projection matrix
  Matrix3 projection = Matrix3::scaling(Vector2::yScale(size.aspectRatio()));
  // create transform matrix
  Matrix3 transform =
      Matrix3::translation(Vector2(x / size.x() * 2, -y / size.y() * 2)) *
      Matrix3::scaling(Vector2(radius / size.x(), radius / size.y()));
  // create circle mesh and drat
  std::get<0>(MeshTools::compile(Primitives::Circle::solid(16),
                                 BufferUsage::StaticDraw))
      .draw(Shaders::Flat2D{}
                .setTransformationProjectionMatrix(projection * transform)
                .setColor(color));
}
}

#endif /* end of include guard: MagnumUtils_h */
