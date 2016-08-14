#ifndef MatView_h
#define MatView_h

#include <Magnum/Buffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/Texture.h>

#include "view/MatViewShader.h"
#include <opencv2/core/core.hpp>

namespace eo {
namespace view {

class MatView {
public:
  void setup();
  void updateTexture(const cv::Mat img);
  void loadTexture();
  void draw();

private:
  Magnum::Mesh mesh;
  Magnum::Texture2D texture;
  Magnum::Buffer buffer;
  MatViewShader shader;
  int numChannels;
};

} // namespace view
} // namespace eo

#endif /* end of include guard: MatView_h */
