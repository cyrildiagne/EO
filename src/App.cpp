#include <iostream>
#include <sstream>

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Renderer.h>
#include <Magnum/Timeline.h>

#include "capture/FlyCaptureCamera.h"
#include "capture/SimulationCapture.h"
#include "tracking/BallTracker.h"
#include "view/Label.h"
#include "view/MatView.h"
#include "view/ballman/BallMan.h"

using namespace Magnum;

class App : public Platform::Application {
public:
  explicit App(const Arguments &arguments);

private:
  void tickEvent() override;
  void drawEvent() override;
  void mouseMoveEvent(MouseMoveEvent &event) override;
  void keyPressEvent(KeyEvent &event) override;
  Label fpsView;
  Timeline timeline;
  MatView MatView;
  std::unique_ptr<AbstractCapture> capture;
  tracking::BallTracker ballTracker;
  Vector2i mouse;
  BallMan ballman;
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
  debugMode = false;
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
  // ballmen setup
  ballman.setup();
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
  Vector2i size = defaultFramebuffer.viewport().size();
  float screenScale = static_cast<float>(size.x()) / 1280;
  // update camera
  if (capture) {
    capture->update();
    if (capture->hasNewImage()) {
      ballTracker.update(capture->getCvImage());
    }
  } else {
    // use a mouse controlled circle if no camera is connected
    ballTracker.circles.clear();
    Vector2i size = defaultFramebuffer.viewport().size();
    float x = (mouse.x() - size.x() * 0.5) / screenScale;
    float y = (mouse.y() - size.y() * 0.5) / screenScale;
    ballTracker.circles.push_back(tracking::Circle(x, y, 100));
  }
  for (const tracking::Circle &c : ballTracker.circles) {
    ballman.update(Vector2(c.x, c.y) * screenScale, c.radius * screenScale,
                   ellapsedTime);
  }
  // update framerate label
  if (debugMode) {
    const float fps = 1.0f / timeline.previousFrameDuration();
    std::ostringstream text;
    text << Int(fps) << "fps" << std::endl
         << "tracking: " << Int(ballTracker.getTrackTime()) << "ms" << std::endl
         << "thresh: " << ballTracker.minHue << "-" << ballTracker.maxHue;
    fpsView.setText(text.str());
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
  for (const tracking::Circle &c : ballTracker.circles) {
    (void)c;
    ballman.draw();
  }
  // swap buffers
  swapBuffers();
  // call next draw
  redraw();
  timeline.nextFrame();
}

void App::mouseMoveEvent(MouseMoveEvent &event) {
  mouse = event.position();
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
    ballTracker.minHue -= 1;
  } else if (event.key() == KeyEvent::Key::Right) {
    ballTracker.minHue += 1;
  } else if (event.key() == KeyEvent::Key::Down) {
    ballTracker.maxHue -= 1;
  } else if (event.key() == KeyEvent::Key::Up) {
    ballTracker.maxHue += 1;
  }
  event.setAccepted(true);
}

int main(int argc, char **argv) {
  App app({argc, argv});
  return app.exec();
}
