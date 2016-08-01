#ifndef BallManShader_h
#define BallManShader_h

#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Context.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Shader.h>
#include <Magnum/Version.h>

using namespace Magnum;

struct BallManShader : AbstractShaderProgram {
  typedef Attribute<0, Vector2> Position;

  BallManShader() {

    MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

    Shader vert{Version::GL330, Shader::Type::Vertex};
    vert.addSource(R"GLSL(
uniform mat3 matrix;
layout(location = 0) in vec4 position;

void main() {
    gl_Position = vec4(matrix*position.xyw, 0.0).xywz;
}
)GLSL").compile();

    //     Shader geom{Version::GL330, Shader::Type::Geometry};
    //     geom.addSource(R"GLSL(
    // layout(lines) in;
    // layout(triangle_strip, max_vertices = 4) out;
    //
    // uniform mat3 matrix;
    //
    // void main() {
    //   vec4 p0 = gl_in[0].gl_Position;
    //   vec4 p1 = gl_in[1].gl_Position;
    //
    //   vec3 up = vec3(0.0, 0.0, 1.0);
    //   vec3 dir = normalize(p1.xyw - p0.xyw);
    //   vec3 right = normalize(cross(dir, up));
    //   right *= 5.0; // thickness
    //
    //   gl_Position = vec4(matrix * (p0.xyw - right), 0.0).xywz;
    //   EmitVertex();
    //   gl_Position = vec4(matrix * (p0.xyw + right), 0.0).xywz;
    //   EmitVertex();
    //   gl_Position = vec4(matrix * (p1.xyw - right), 0.0).xywz;
    //   EmitVertex();
    //   gl_Position = vec4(matrix * (p1.xyw + right), 0.0).xywz;
    //   EmitVertex();
    //
    // EndPrimitive();
    // }
    //     )GLSL")
    //         .compile();

    Shader frag{Version::GL330, Shader::Type::Fragment};
    frag.addSource(R"GLSL(
uniform vec4 color;
out vec4 fragmentColor;

void main() {
    fragmentColor = color;
}
)GLSL").compile();

    attachShader(vert);
    // attachShader(geom);
    attachShader(frag);
    link();
  }

  BallManShader &setTransformationProjectionMatrix(const Matrix3 &matrix) {
    int loc = uniformLocation("matrix");
    setUniform(loc, matrix);
    return *this;
  }

  BallManShader &setColor(const Color4 &color) {
    int loc = uniformLocation("color");
    setUniform(loc, color);
    return *this;
  }
};

#endif /* end of include guard: BallManShader_h */
