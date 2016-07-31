#include "CameraViewShader.h"

#include <Corrade/Utility/Resource.h>
#include <Magnum/Context.h>
#include <Magnum/Shader.h>
#include <Magnum/Version.h>

CameraViewShader::CameraViewShader() {
  MAGNUM_ASSERT_VERSION_SUPPORTED(Version::GL330);

  const Utility::Resource rs{"assets"};

  Shader vert{Version::GL330, Shader::Type::Vertex};
  Shader frag{Version::GL330, Shader::Type::Fragment};

  vert.addSource(rs.get("CameraViewShader.vert"));
  frag.addSource(rs.get("CameraViewShader.frag"));

  CORRADE_INTERNAL_ASSERT_OUTPUT(Shader::compile({vert, frag}));

  attachShaders({vert, frag});

  CORRADE_INTERNAL_ASSERT_OUTPUT(link());

  setUniform(uniformLocation("textureData"), TextureLayer);
}
