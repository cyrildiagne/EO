#include "MatView.h"

#include "configure.h"
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Primitives/Square.h>
#include <Magnum/TextureFormat.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData2D.h>
#include <iostream>

using namespace Magnum;
using namespace Magnum::Primitives;

void MatView::setup() {
  // setup mesh
  const Trade::MeshData2D square =
      Primitives::Square::solid(Square::TextureCoords::Generate);
  buffer.setData(
      MeshTools::interleave(square.positions(0), square.textureCoords2D(0)),
      BufferUsage::StaticDraw);
  mesh.setPrimitive(square.primitive())
      .setCount(square.positions(0).size())
      .addVertexBuffer(buffer, 0, MatViewShader::Position{},
                       MatViewShader::TextureCoordinates{});
}

void MatView::updateTexture(const cv::Mat img) {
  // retrieve image size
  const Vector2i size(img.cols, img.rows);
  // copy data to magnum raw buffer
  std::size_t dataSize = img.cols * img.rows * 3;
  Containers::Array<char> data{dataSize};
  std::copy_n(img.ptr(), data.size(), data.begin());
  // retrieve parsed ImageData from flycapture image
  PixelStorage storage;
  Trade::ImageData2D image{storage, PixelFormat::RGB, PixelType::UnsignedByte,
                           size, std::move(data)};
  // Set texture data and parameters
  texture.setWrapping(Sampler::Wrapping::ClampToEdge)
      .setMagnificationFilter(Sampler::Filter::Linear)
      .setMinificationFilter(Sampler::Filter::Linear)
      .setStorage(1, TextureFormat::RGB8, image.size())
      .setSubImage(0, {}, image);
}

void MatView::loadTexture() {
  // load the plugin
  PluginManager::Manager<Trade::AbstractImporter> manager{
      MAGNUM_PLUGINS_IMPORTER_DIR};
  std::unique_ptr<Trade::AbstractImporter> importer =
      manager.loadAndInstantiate("TgaImporter");
  if (!importer)
    std::exit(1);
  // load texture
  const Utility::Resource rs{"assets"};
  if (!importer->openData(rs.getRaw("stone.tga")))
    std::exit(2);
  // retrieve parsed ImageData from importer
  std::optional<Trade::ImageData2D> image = importer->image2D(0);
  CORRADE_INTERNAL_ASSERT(image);
  // Set texture data and parameters
  texture.setWrapping(Sampler::Wrapping::ClampToEdge)
      .setMagnificationFilter(Sampler::Filter::Linear)
      .setMinificationFilter(Sampler::Filter::Linear)
      .setStorage(1, TextureFormat::RGB8, image->size())
      .setSubImage(0, {}, *image);
}

void MatView::draw() {
  shader.setTexture(texture);
  mesh.draw(shader);
}
