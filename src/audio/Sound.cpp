#include "Sound.h"

#include <iostream>

#include "configure.h"

namespace eo {
namespace audio {

using namespace Magnum;

void Sound::setup() {
  PluginManager::Manager<Audio::AbstractImporter> audioManager{
      MAGNUM_PLUGINS_AUDIOIMPORTER_DIR};
  // load plugin
  std::unique_ptr<Audio::AbstractImporter> wavImporter =
      audioManager.loadAndInstantiate("WavAudioImporter");
  if (!wavImporter) {
    std::cout << "could not find audio wav importer" << std::endl;
    return;
  }
  // load resource
  Utility::Resource rs("assets");
  // if (!wavImporter->openData(rs.getRaw("48310__dkifer__handclap.wav"))) {
  if (!wavImporter->openData(rs.getRaw("clap-bathroom-02.wav"))) {
    std::cout << "could not open wav file" << std::endl;
    return;
  }
  // add to buffer
  bufferData = wavImporter->data();
  buffer.setData(wavImporter->format(), bufferData, wavImporter->frequency());
}

void Sound::play(Clip clip) {
  std::shared_ptr<Magnum::Audio::Source> source = nullptr;
  for (size_t i = 0; i < sources.size(); i++) {
    if (sources[i]->state() != Magnum::Audio::Source::State::Playing) {
      source = sources[i];
      break;
    }
  }
  // setup new audio source
  if (source == nullptr) {
    source = std::make_shared<Magnum::Audio::Source>();
    source->setBuffer(&buffer);
    source->setLooping(false);
    sources.push_back(source);
  }
  source->play();
}

} // namespace audio
} // namespace eo
