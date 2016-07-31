#ifndef Label_h
#define Label_h

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>
#include <Magnum/Text/Renderer.h>

class Label {
public:
  Label();
  void setup();
  void setText(std::string text) { renderer->render(text); }
  void draw();

private:
  void loadFont();

  // properties
  Magnum::PluginManager::Manager<Magnum::Text::AbstractFont> manager;
  std::unique_ptr<Magnum::Text::AbstractFont> font;

  Magnum::Text::DistanceFieldGlyphCache cache;
  std::unique_ptr<Magnum::Text::Renderer2D> renderer;
  Magnum::Shaders::DistanceFieldVector2D shader;

  Magnum::Matrix3 projection;
};

#endif /* end of include guard: Label_h */
