#ifndef CameraView_h
#define CameraView_h

#include <Magnum/Buffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/Texture.h>

#include "CameraViewShader.h"
#include "FlyCaptureCamera.h"

class CameraView {
public:
  void setup();
  void updateTexture(const FlyCapture2::Image &fcImage);
  void loadTexture();
  void draw();

private:
  Magnum::Mesh mesh;
  Magnum::Texture2D texture;
  Magnum::Buffer buffer;
  CameraViewShader shader;
};

#endif /* end of include guard: CameraView_h */
