#include "BallMan.h"

void BallMan::setup() {
  std::tie(mesh, vertices, indices) = MeshTools::compile(
      Primitives::Circle::wireframe(32), BufferUsage::StaticDraw);
  leftArm.setup(20);
  rightArm.setup(20);
  leftLeg.setup();
  rightLeg.setup();
}

void BallMan::update(Vector2 p, float radius, float t) {
  leftArm.update(Vector2(p.x() - radius, p.y()), t);
  rightArm.update(Vector2(p.x() + radius, p.y()), t);

  Vector2 legsOffset =
      Matrix3::rotation(60.0_degf).transformVector({radius, 0});
  leftLeg.update(p + legsOffset, t);
  Vector2 rightLegOffset{p.x() - legsOffset.x(), p.y() + legsOffset.y()};
  rightLeg.update(rightLegOffset, t);
  // retrieve viewport size
  Vector2i size = defaultFramebuffer.viewport().size();
  // update transform
  float diam = radius * 2;
  transform = Matrix3::translation(
                  Vector2(p.x() / size.x() * 2, -p.y() / size.y() * 2)) *
              Matrix3::scaling(Vector2(diam / size.x(), diam / size.y()));
}

void BallMan::draw() {
  // retrieve projection matrix
  Vector2i size = defaultFramebuffer.viewport().size();
  Matrix3 projection = Matrix3::scaling(Vector2::yScale(size.aspectRatio()));
  shader.setTransformationProjectionMatrix(projection * transform)
      .setColor(Color3{1.f, 1.f, 1.f});
  // draw circle
  mesh.draw(shader);
  leftArm.draw();
  rightArm.draw();
  leftLeg.draw();
  rightLeg.draw();
}
