#include "Line.h"

#include <Magnum/DefaultFramebuffer.h>
#
Line::Line() : thickness(15.f), scale{1.f, 1.f}, position{0.f, 0.f} {
  mesh.setPrimitive(MeshPrimitive::TriangleStrip);
}

void Line::setPoints(const std::vector<Vector2> &pts) {
  // create ribbon
  const std::vector<Vector2> &ribbon = getRibbon(pts);
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

std::vector<Vector2> Line::getRibbon(const std::vector<Vector2> &pts) {
  std::vector<Vector2> ribbon;
  for (size_t i = 0; i < pts.size() - 1; i++) {
    const auto &p0 = pts[i];
    const auto &p1 = pts[i + 1];
    Vector2 offset = (p1 - p0).normalized().perpendicular() * thickness * 0.5;
    ribbon.push_back(p0 - offset);
    ribbon.push_back(p0 + offset);
  }
  // last point
  const auto &p0 = pts.back();
  const auto &p1 = pts[pts.size() - 2];
  Vector2 offset = (p0 - p1).normalized().perpendicular() * thickness * 0.5;
  ribbon.push_back(p0 - offset);
  ribbon.push_back(p0 + offset);
  // return vector
  return ribbon;
}
