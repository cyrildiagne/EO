#ifndef BallMan_h
#define BallMan_h

#include <Magnum/Buffer.h>
#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Trade/MeshData2D.h>

using namespace Magnum;

class BallMan {
public:
  static void draw(double x, double y, double radius) {
    // retrieve projection matrix
    Matrix3 projection = Matrix3::scaling(Vector2::yScale(
        Vector2(defaultFramebuffer.viewport().size()).aspectRatio()));
    // draw circle
    float scale = 0.0065;
    Matrix3 transformation = Matrix3::scaling(Vector2(scale, scale)) *
                             Matrix3::translation(Vector2(-x, -y)) *
                             Matrix3::scaling(Vector2(radius, radius));
    // draw circle
    std::get<0>(MeshTools::compile(Primitives::Circle::wireframe(16),
                                   BufferUsage::StaticDraw))
        .draw(
            Shaders::Flat2D{}
                .setTransformationProjectionMatrix(projection * transformation)
                .setColor(Color3::red()));
  }
};

#endif /* end of include guard: BallMan_h */
