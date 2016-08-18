#include <iostream>
#include <sstream>

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Renderer.h>
#include <Magnum/Timeline.h>

#include "capture/FlyCaptureCamera.h"
#include "capture/SimulationCapture.h"
#include "tracking/BallTracker.h"
#include "utils.h"
#include "view/Label.h"
#include "view/MatView.h"
#include "view/ballman/BallMan.h"

using namespace Magnum;

namespace eo {

using namespace view;
using namespace tracking;
using namespace capture;

class App : public Platform::Application {
public:
  explicit App(const Arguments &arguments);

private:
  void tickEvent() override;
  void drawEvent() override;
  void mouseMoveEvent(MouseMoveEvent &event) override;
  void keyPressEvent(KeyEvent &event) override;
  //
  void updateBallMen(const std::vector<FollowedCircle> &circles);
  // properties
  Label fpsView;
  Timeline timeline;
  MatView MatView;
  std::unique_ptr<AbstractCapture> capture;
  BallTracker ballTracker;
  std::map<std::string, std::shared_ptr<BallMan>> ballmen;
  bool debugMode;
  float ellapsedTime;
};

App::App(const Arguments &arguments)
    : Platform::Application(
          arguments, Configuration()
                         .setWindowFlags(Configuration::WindowFlag::Fullscreen)
                         .setTitle("EO beta")
                         .setSize({1920, 1080})),
      capture(new SimulationCapture) {
  ellapsedTime = 0.f;
  debugMode = true;
  fpsView.setup();
  // setup flycapture cam
  bool isAvailable = capture->setup();
  if (!isAvailable) {
    capture = nullptr;
  }
  // camera view
  MatView.setup();
  // ball tracker
  ballTracker.setup();
  // enable alpha blending
  Renderer::enable(Renderer::Feature::Blending);
  Renderer::setBlendFunction(Renderer::BlendFunction::SourceAlpha,
                             Renderer::BlendFunction::OneMinusSourceAlpha);
  Renderer::setBlendEquation(Renderer::BlendEquation::Add,
                             Renderer::BlendEquation::Add);
  // limit framrate to 60hz
  setMinimalLoopPeriod(16);
  timeline.start();
}

void App::tickEvent() {
  ellapsedTime += timeline.previousFrameDuration();
  // update camera
  if (capture) {
    capture->update();
    if (capture->hasNewImage()) {
      ballTracker.update(capture->getCvImage(), debugMode);
    }
  }
  // update characters
  updateBallMen(ballTracker.follower.circles);
  // update framerate label
  if (debugMode) {
    const float fps = 1.0f / timeline.previousFrameDuration();
    std::ostringstream text;
    text << Int(fps) << "fps" << std::endl
         << "tracking: " << Int(ballTracker.getTrackTime()) << "ms" << std::endl
         << "detected: " << ballTracker.follower.circles.size() << std::endl
         << "balls: " << ballmen.size() << std::endl
         << "thresh: " << ballTracker.detector.minHue << "-"
         << ballTracker.detector.maxHue;
    fpsView.setText(text.str());
  }
}

void App::updateBallMen(const std::vector<FollowedCircle> &circles) {
  Vector2i size = defaultFramebuffer.viewport().size();
  float screenScale = static_cast<float>(size.x()) / 1280;
  // save all the ballmen that have been updated here
  auto deadBallMen = ballmen;
  // loop through all tracked circles
  for (const tracking::FollowedCircle &c : circles) {
    auto ball = ballmen.find(c.label);
    // retrieve position & radius
    const Vector2 position = Vector2(c.circle.x, c.circle.y) * screenScale;
    const float radius = c.circle.radius * screenScale;
    // circle needs a new character
    if (ball == ballmen.end()) {
      ballmen[c.label] = std::shared_ptr<BallMan>(new BallMan);
      if (debugMode) {
        Color3 color{utils::random(1.f), utils::random(1.f),
                     utils::random(1.f)};
        ballmen[c.label]->setup(position, radius, color);
      } else {
        ballmen[c.label]->setup(position, radius);
      }
      ball = ballmen.find(c.label);
    } else {
      // otherwise remove the ball from the deadballs list
      auto it = deadBallMen.find(ball->first);
      if (it != deadBallMen.end()) {
        deadBallMen.erase(it);
      }
      // and sync visibility status
      ball->second->visible = (c.numUpdatesMissing == 0);
    }
    // update ball
    ball->second->update(position, radius, ellapsedTime);
  }
  // remove characters that don't have circles anymore
  for (const auto &b : deadBallMen) {
    auto it = ballmen.find(b.first);
    if (it != ballmen.end()) {
      ballmen.erase(it);
    }
  }
}

void App::drawEvent() {
  // update display
  defaultFramebuffer.clear(FramebufferClear::Color);
  // draw our content
  if (capture) {
    if (capture->hasNewImage()) {
      MatView.updateTexture(ballTracker.getCurrFrame());
    }
    MatView.draw();
  }
  // draw debug data
  if (debugMode) {
    fpsView.draw();
  }
  // draw ballmen
  for (auto &b : ballmen) {
    b.second->draw();
  }
  // swap buffers
  swapBuffers();
  // call next draw
  redraw();
  timeline.nextFrame();
}

void App::mouseMoveEvent(MouseMoveEvent &event) {
  Vector2i mouse = event.position();
  Vector2i size = defaultFramebuffer.viewport().size();
  float screenScale = static_cast<float>(size.x()) / 1280;
  float x = static_cast<float>(size.x() - mouse.x()) / screenScale;
  float y = static_cast<float>(mouse.y()) / screenScale;
  SimulationCapture::mouse.x = x;
  SimulationCapture::mouse.y = y;
  event.setAccepted();
}

void App::keyPressEvent(KeyEvent &event) {
  if (event.key() == KeyEvent::Key::S) {
    // save image
    capture->saveImage(0.5);
  } else if (event.key() == KeyEvent::Key::Space) {
    ballTracker.setNextFrameId();
  } else if (event.key() == KeyEvent::Key::D) {
    debugMode = !debugMode;
  } else if (event.key() == KeyEvent::Key::Left) {
    ballTracker.detector.minHue -= 1;
  } else if (event.key() == KeyEvent::Key::Right) {
    ballTracker.detector.minHue += 1;
  } else if (event.key() == KeyEvent::Key::Down) {
    ballTracker.detector.maxHue -= 1;
  } else if (event.key() == KeyEvent::Key::Up) {
    ballTracker.detector.maxHue += 1;
  }
  event.setAccepted(true);
}
}

int main(int argc, char **argv) {
  eo::App app({argc, argv});
  return app.exec();
}
