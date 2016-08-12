#include "view/ballman/Line.h"

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/MeshTools/Interleave.h>

Line::Line()
    : thickness(15.f), color{1.f, 1.f, 1.f}, scale{1.f, 1.f},
      position{0.f, 0.f} {
  mesh.setPrimitive(MeshPrimitive::TriangleStrip);
}

void Line::setPoints(const std::vector<Vector2> &pts) {
  std::vector<Vector2> pos;
  for (const auto &pt : pts) {
    pos.push_back(pt);
    pos.push_back(pt);
  }
  // create normals
  const std::vector<Vector2> &normals = computeNormals(pts);
  // update buffers
  buffer.setData(MeshTools::interleave(pos, normals), BufferUsage::DynamicDraw);
  mesh.setCount(pos.size())
      .addVertexBuffer(buffer, 0, LineShader::Position(), LineShader::Normal());
}

void Line::draw() {
  Vector2i size = defaultFramebuffer.viewport().size();
  // get projection
  Matrix3 projection = Matrix3::scaling(Vector2::yScale(size.aspectRatio()));
  Matrix3 transform =
      Matrix3::translation(
          Vector2{position.x() / size.x(), -position.y() / size.y()} * 2) *
      Matrix3::scaling(Vector2{scale.x() / size.x(), -scale.x() / size.y()} *
                       2);
  // update shader
  shader.setTransformationProjectionMatrix(projection * transform)
      .setColor(color)
      .setThickness(thickness);
  // draw mesh
  mesh.draw(shader);
}

std::vector<Vector2> Line::computeNormals(const std::vector<Vector2> &pts) {
  std::vector<Vector2> normals;
  for (size_t i = 0; i < pts.size() - 1; i++) {
    const auto &p0 = pts[i];
    const auto &p1 = pts[i + 1];
    Vector2 offset = (p1 - p0).normalized().perpendicular();
    normals.push_back(-offset * 0.5);
    normals.push_back(offset * 0.5);
  }
  // last point
  const auto &p0 = pts.back();
  const auto &p1 = pts[pts.size() - 2];
  Vector2 offset = (p0 - p1).normalized().perpendicular();
  normals.push_back(-offset * 0.5);
  normals.push_back(offset * 0.5);
  // return vector
  return normals;
}
