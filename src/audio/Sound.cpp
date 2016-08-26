#include "Sound.h"

#include <iostream>

namespace eo {
namespace audio {

using namespace Magnum;

void Sound::setup() {
  // load plugin
  wavImporter = audioManager.loadAndInstantiate("WavAudioImporter");
  if (!wavImporter) {
    std::cout << "could not find audio wav importer" << std::endl;
    return;
  }
  // load resources
  addClip(Clip::CLAP1, "48310__dkifer__handclap.wav");
  addClip(Clip::CLAP2, "clap-bathroom-02.wav");
}

void Sound::addClip(eo::audio::Sound::Clip clipId, std::string filename) {
  Utility::Resource rs("assets");
  if (!wavImporter->openData(rs.getRaw(filename))) {
    std::cout << "could not open wav file" << std::endl;
    return;
  }
  // add to buffer
  clips[clipId] = AudioClip();
  clips[clipId].bufferData = wavImporter->data();
  clips[clipId].buffer.setData(wavImporter->format(), clips[clipId].bufferData,
                               wavImporter->frequency());
}

void Sound::play(Clip clip) {
  // find an idle audio source
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
    source->setLooping(false);
    sources.push_back(source);
  }
  source->setBuffer(&clips[clip].buffer);
  source->play();
}

} // namespace audio
} // namespace eo
