#ifndef audio_Sound_h
#define audio_Sound_h

#include <Corrade/Containers/Containers.h>
#include <Corrade/PluginManager/PluginManager.h>
#include <Magnum/Audio/AbstractImporter.h>
#include <Magnum/Audio/Buffer.h>
#include <Magnum/Audio/Context.h>
#include <Magnum/Audio/Playable.h>
#include <Magnum/Audio/Source.h>

#include "configure.h"
#include "tracking/BallTracker.h"
#include "view/ballman/BallMan.h"
#include <map>

namespace eo {
namespace audio {

class AudioClip {
public:
  AudioClip() {}
  Corrade::Containers::Array<char> bufferData;
  Magnum::Audio::Buffer buffer;
};

class Sound {
public:
  Sound() : audioManager{MAGNUM_PLUGINS_AUDIOIMPORTER_DIR} {}
  enum Clip { CLAP1, CLAP2, CLAP3, NEW1, NEW2, YEYE };
  void setup();
  void play(Clip clip);

  void addClip(Clip clipId, std::string filename);
  std::shared_ptr<Audio::AbstractImporter> wavImporter;
  std::map<Clip, AudioClip> clips;

private:
  Magnum::Audio::Context context;
  std::vector<std::shared_ptr<Magnum::Audio::Source>> sources;
  PluginManager::Manager<Audio::AbstractImporter> audioManager;
};

} // namespace audio
} // namespace eo

#endif /* end of include guard: audio_Sound_h */
