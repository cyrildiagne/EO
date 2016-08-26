#ifndef view_ballmencontroller_h
#define view_ballmencontroller_h

#include <map>

#include "audio/Sound.h"
#include "tracking/BallTracker.h"
#include "view/ballman/BallMan.h"
#include "view/fx/Clap.h"

namespace eo {
namespace view {

class BallMenController {
public:
  // ~BallMenController();
  BallMenController() : numUpdateSinceLastClap(0) { setupClap(); };
  void update(const std::vector<tracking::FollowedCircle> &circles, double dt);
  void draw();

  int getNum() { return ballmen.size(); }

private:
  std::map<std::string, std::shared_ptr<BallMan>> ballmen;

  void updateClaps();
  void checkMatch(Leg &l1, Leg &l2);
  bool updateLegTarget(Leg &leg);

  void setupClap();
  void playClap();
  Clap clap;
  audio::Sound sound;
  long unsigned int numUpdateSinceLastClap;
};

} // namespace view
} // namespace eo

#endif /* end of include guard: view_ballmencontroller_h */
