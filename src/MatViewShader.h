#ifndef MatViewShader_h
#define MatViewShader_h

#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Texture.h>

using namespace Magnum;

class MatViewShader : public AbstractShaderProgram {
public:
  typedef Attribute<0, Vector2> Position;
  typedef Attribute<1, Vector2> TextureCoordinates;

  explicit MatViewShader();

  MatViewShader &setTexture(Texture2D &texture) {
    texture.bind(TextureLayer);
    return *this;
  }

private:
  enum : Int { TextureLayer = 0 };
};

#endif /* end of include guard: MatViewShader_h */