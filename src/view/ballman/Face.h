#ifndef view_ballman_Face_h
#define view_ballman_Face_h

#include <memory>

#include <Magnum/Buffer.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Flat.h>

#include "view/primitives/Line.h"

using namespace Magnum;

namespace eo {
namespace view {

class Face {
public:
  enum Expression { None, Neutral, Happy, Laughing, Extatic };

  Face() : currentExpression(None), newExpression(Neutral) {}
  void setup();
  void setExpression(Expression expr);
  void update(Vector2 p, Vector2 s);
  void draw();

  void setColor(Magnum::Color3 color);
  Expression currentExpression;

private:
  std::shared_ptr<Line> leftEye;
  std::shared_ptr<Line> rightEye;
  std::shared_ptr<Line> mouth;

  Expression newExpression;
  void applyNewExpression();
};

} // namespace view
} // namespace eo

#endif /* end of include guard: view_ballman_Face_h */
