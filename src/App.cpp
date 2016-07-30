#include <iostream>

#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

#include "CameraView.h"
#include "FlyCaptureCamera.h"

using namespace Magnum;

class App : public Platform::Application {
public:
  explicit App(const Arguments &arguments);

private:
  void drawEvent() override;
  void mouseMoveEvent(MouseMoveEvent &event) override;
  // camera view
  CameraView cameraView;
  // flycapture camera
  FlyCaptureCamera fcCamera;
};

App::App(const Arguments &arguments)
    : Platform::Application(
          arguments,
          Configuration().setTitle("EO beta").setSize({1280, 1024})) {
  // setup flycapture cam
  fcCamera.setup();
  // camera view
  cameraView.setup();
  // limit at 60hz
  setSwapInterval(1);
}

void App::drawEvent() {
  fcCamera.update();
  cameraView.updateTexture(fcCamera.rawImage);
  // update display
  defaultFramebuffer.clear(FramebufferClear::Color);
  cameraView.draw();
  swapBuffers();
  // call next draw
  redraw();
}

void App::mouseMoveEvent(MouseMoveEvent &event) { (void)event; }

MAGNUM_APPLICATION_MAIN(App)
