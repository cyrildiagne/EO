#ifndef BallMan_h
#define BallMan_h

#include "view/ballman/Face.h"
#include "view/ballman/Leg.h"
#include "view/primitives/Circle.h"

namespace eo {
namespace view {

class BallMan {
public:
  BallMan(){};
  void setup();
  void update(Vector2 p, float radius, float t);
  void draw();

  void setColor(Magnum::Color3 color);

  // helps keeping track of characters without tracked circles
  bool alive;

private:
  Circle body;
  Leg leftArm;
  Leg rightArm;
  Leg leftLeg;
  Leg rightLeg;
  Face face;
};

} // namespace view
} // namespace eo

#endif /* end of include guard: BallMan_h */
