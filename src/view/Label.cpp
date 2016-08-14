#include "view/Label.h"
#include "configure.h"
#include <Magnum/DefaultFramebuffer.h>

using namespace Magnum;

namespace eo {
namespace view {

Label::Label()
    : manager(MAGNUM_PLUGINS_FONT_DIR),
      cache(Vector2i(2048), Vector2i(512), 22) {}

void Label::setup() {
  loadFont();
  projection = Matrix3::scaling(Vector2::yScale(
      Vector2(defaultFramebuffer.viewport().size()).aspectRatio()));
}

void Label::loadFont() {
  // Load FreeTypeFont plugin
  font = manager.loadAndInstantiate("FreeTypeFont");
  if (!font) {
    Error() << "Cannot load FreeTypeFont plugin";
    return;
  }

  // Open the font and fill glyph cache
  Utility::Resource rs("assets");
  if (!font->openSingleData(rs.getRaw("DejaVuSans.ttf"), 110.0f)) {
    Error() << "Cannot open font file";
    return;
  }

  font->fillGlyphCache(cache, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRST"
                              "UVWXYZ0123456789:-+,.!° ");
  renderer.reset(
      new Text::Renderer2D(*font, cache, 0.035f, Text::Alignment::TopRight));
  renderer->reserve(100, BufferUsage::DynamicDraw, BufferUsage::StaticDraw);
}

void Label::draw() {
  shader.setVectorTexture(cache.texture());
  shader
      .setTransformationProjectionMatrix(
          projection *
          Matrix3::translation(1.0f / projection.rotationScaling().diagonal()))
      .setColor(Color3{1.0f})
      .setOutlineRange(0.5f, 1.0f)
      .setSmoothness(0.075f);
  renderer->mesh().draw(shader);
}

} // namespace view
} // namespace eo
