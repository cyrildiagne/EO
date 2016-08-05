#include <iostream>
#include <sstream>

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Renderer.h>
#include <Magnum/Timeline.h>

#include "BallMan.h"
#include "BallTracker.h"
#include "FlyCaptureCamera.h"
#include "Label.h"
#include "MatView.h"

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
  std::unique_ptr<FlyCaptureCamera> fcCamera;
  BallTracker ballTracker;
  Vector2i mouse;
  BallMan ballman;
  float ellapsedTime;
};

App::App(const Arguments &arguments)
    : Platform::Application(
          arguments, Configuration().setTitle("EO beta").setSize({1280, 1024})),
      fcCamera(new FlyCaptureCamera) {
  ellapsedTime = 0.f;
  fpsView.setup();
  // setup flycapture cam
  bool isAvailable = fcCamera->setup();
  if (!isAvailable) {
    fcCamera = nullptr;
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
  // update camera
  if (fcCamera) {
    fcCamera->update();
    if (fcCamera->hasNewImage()) {
      ballTracker.update(fcCamera->getCvImage());
    }
  } else {
    // use a mouse controlled circle if no camera is connected
    ballTracker.circles.clear();
    Vector2i size = defaultFramebuffer.viewport().size();
    float x = mouse.x() - size.x() * 0.5;
    float y = mouse.y() - size.y() * 0.5;
    ballTracker.circles.push_back(Circle(x, y, 100));
  }
  for (const Circle &c : ballTracker.circles) {
    ballman.update(Vector2{c.x, c.y}, c.radius, ellapsedTime);
  }
  // update framerate label
  const float fps = 1.0f / timeline.previousFrameDuration();
  std::ostringstream text;
  text << Int(fps) << "fps" << std::endl
       << "tracking: " << Int(ballTracker.trackTime) << "ms";
  fpsView.setText(text.str());
}

void App::drawEvent() {
  // update display
  defaultFramebuffer.clear(FramebufferClear::Color);
  // draw our content
  if (fcCamera) {
    if (fcCamera->hasNewImage()) {
      MatView.updateTexture(ballTracker.getImage());
    }
    MatView.draw();
  }
  fpsView.draw();
  // draw ballmen
  for (const Circle &c : ballTracker.circles) {
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
    fcCamera->saveImage(0.5);
  };
  event.setAccepted(true);
}

int main(int argc, char **argv) {
  App app({argc, argv});
  return app.exec();
}
