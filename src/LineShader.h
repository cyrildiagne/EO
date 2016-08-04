#ifndef LineShader_h
#define LineShader_h

#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Context.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Shader.h>
#include <Magnum/Version.h>

using namespace Magnum;

struct LineShader : AbstractShaderProgram {
  typedef Attribute<0, Vector2> Position;
  typedef Attribute<1, Vector2> Normal;

  LineShader() {

    MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

    Shader vert{Version::GL330, Shader::Type::Vertex};
    vert.addSource(R"GLSL(
uniform mat3 matrix;
uniform float thickness;
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

out vec2 vNormal;

void main() {
  gl_Position = vec4(matrix*(position.xyw+normal.xyz*thickness), 0.0).xywz;
  vNormal = normal.xy;
}
)GLSL").compile();

    Shader frag{Version::GL330, Shader::Type::Fragment};
    frag.addSource(R"GLSL(
uniform vec4 color;
out vec4 fragmentColor;
in vec2 vNormal;

void main() {
  float dist = sqrt(vNormal.x*vNormal.x+vNormal.y*vNormal.y)*2;
  float alpha = 1.f;
  float feather = 0.075;
  if (1-dist < feather) {
    alpha = (1-dist) / feather;
  }
  fragmentColor = vec4(color.rgb, alpha);
}
)GLSL").compile();

    attachShader(vert);
    attachShader(frag);
    link();
  }

  LineShader &setTransformationProjectionMatrix(const Matrix3 &matrix) {
    int loc = uniformLocation("matrix");
    setUniform(loc, matrix);
    return *this;
  }

  LineShader &setColor(const Color4 &color) {
    int loc = uniformLocation("color");
    setUniform(loc, color);
    return *this;
  }

  LineShader &setThickness(const float &thickness) {
    int loc = uniformLocation("thickness");
    setUniform(loc, thickness);
    return *this;
  }
};

#endif /* end of include guard: LineShader_h */
