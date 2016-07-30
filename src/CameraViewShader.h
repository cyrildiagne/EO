#ifndef TexturedTriangleShader_h
#define TexturedTriangleShader_h

#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Texture.h>

using namespace Magnum;

class CameraViewShader : public AbstractShaderProgram {
public:
  typedef Attribute<0, Vector2> Position;
  typedef Attribute<1, Vector2> TextureCoordinates;

  explicit CameraViewShader();

  CameraViewShader &setTexture(Texture2D &texture) {
    texture.bind(TextureLayer);
    return *this;
  }

private:
  enum : Int { TextureLayer = 0 };
};

#endif /* end of include guard: TexturedTriangleShader_h */
