#include "Line.h"

#include <Magnum/DefaultFramebuffer.h>
#
Line::Line() : thickness(10.f), scale{1.f, 1.f}, position{0.f, 0.f} {
  mesh.setPrimitive(MeshPrimitive::TriangleStrip);
}

void Line::setPoints(const std::vector<Vector2> &pts) {
  // create ribbon
  std::vector<Vector2> ribbon;
  for (const auto &pt : pts) {
    ribbon.push_back(Vector2(pt.x() - thickness * 0.5, pt.y()));
    ribbon.push_back(Vector2(pt.x() + thickness * 0.5, pt.y()));
  }
  // update buffers
  buffer.setData(ribbon, BufferUsage::DynamicDraw);
  mesh.setCount(ribbon.size())
      .addVertexBuffer(buffer, 0, Shaders::Flat2D::Position());
}

void Line::draw() {
  Vector2i size = defaultFramebuffer.viewport().size();
  // get projection
  Matrix3 projection = Matrix3::scaling(Vector2::yScale(size.aspectRatio()));
  Matrix3 transform =
      Matrix3::translation(
          Vector2{position.x() / size.x() * 2, -position.y() / size.y() * 2}) *
      Matrix3::scaling(
          Vector2{scale.x() / size.x() * 2, -scale.x() / size.y() * 2});
  // update shader
  shader.setTransformationProjectionMatrix(projection * transform)
      .setColor(Color3(1.f, 1.f, 1.f));
  // draw mesh
  mesh.draw(shader);
}
