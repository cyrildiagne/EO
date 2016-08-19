#ifndef view_ballmencontroller_h
#define view_ballmencontroller_h

#include <map>

#include <Magnum/Audio/AbstractImporter.h>
#include <Magnum/Audio/Buffer.h>
#include <Magnum/Audio/Context.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/Audio/Source.h>

#include "tracking/BallTracker.h"
#include "view/ballman/BallMan.h"

namespace eo {
namespace view {

class BallMenController {
public:
  BallMenController() { setupClap(); };
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

  Corrade::Containers::Array<char> bufferData;
  // Magnum::Audio::Playable2D clap;
  Magnum::Audio::Context context;
  Magnum::Audio::Buffer testBuffer;
  Magnum::Audio::Source source;
};

} // namespace view
} // namespace eo

#endif /* end of include guard: view_ballmencontroller_h */
