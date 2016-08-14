#ifndef MatViewShader_h
#define MatViewShader_h

#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Texture.h>

using namespace Magnum;

namespace eo {
namespace view {

class MatViewShader : public AbstractShaderProgram {
public:
  typedef Attribute<0, Vector2> Position;
  typedef Attribute<1, Vector2> TextureCoordinates;

  explicit MatViewShader();

  MatViewShader &setTexture(Texture2D &texture) {
    texture.bind(TextureLayer);
    return *this;
  }

  MatViewShader &setTransformationProjectionMatrix(const Matrix3 &matrix) {
    int loc = uniformLocation("matrix");
    setUniform(loc, matrix);
    return *this;
  }

  MatViewShader &setNumChannels(int numChannels) {
    int loc = uniformLocation("numChannels");
    setUniform(loc, numChannels);
    return *this;
  }

private:
  enum : Int { TextureLayer = 0 };
};

} // namespace view
} // namespace eo

#endif /* end of include guard: MatViewShader_h */
