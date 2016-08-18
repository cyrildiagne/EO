#ifndef view_ballmencontroller_h
#define view_ballmencontroller_h

#include <map>

#include "tracking/BallTracker.h"
#include "view/ballman/BallMan.h"

namespace eo {
namespace view {

class BallMenController {
public:
  BallMenController(){};
  void update(const std::vector<tracking::FollowedCircle> &circles, double dt);
  void draw();
  int getNum() { return ballmen.size(); }

private:
  std::map<std::string, std::shared_ptr<BallMan>> ballmen;
};

} // namespace view
} // namespace eo

#endif /* end of include guard: view_ballmencontroller_h */
