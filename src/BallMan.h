#ifndef BallMan_h
#define BallMan_h

#include "BallManShader.h"
#include "Body.h"
#include "Leg.h"

class BallMan {
public:
  void setup();
  void update(Vector2 p, float radius, float t);
  void draw();

private:
  Body body;
  Leg leftArm;
  Leg rightArm;
  Leg leftLeg;
  Leg rightLeg;
};

#endif /* end of include guard: BallMan_h */
