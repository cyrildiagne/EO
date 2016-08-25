#ifndef audio_Sound_h
#define audio_Sound_h

#include <Corrade/Containers/Containers.h>
#include <Corrade/PluginManager/PluginManager.h>
#include <Magnum/Audio/AbstractImporter.h>
#include <Magnum/Audio/Buffer.h>
#include <Magnum/Audio/Context.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/Audio/Source.h>

#include "tracking/BallTracker.h"
#include "view/ballman/BallMan.h"
#include <map>

namespace eo {
namespace audio {

class Sound {
public:
  enum Clip { CLAP1, CLAP2 };
  void setup();
  void play(Clip clip);

private:
  Corrade::Containers::Array<char> bufferData;
  Magnum::Audio::Context context;
  Magnum::Audio::Buffer buffer;
  std::vector<std::shared_ptr<Magnum::Audio::Source>> sources;
};

} // namespace audio
} // namespace eo

#endif /* end of include guard: audio_Sound_h */
