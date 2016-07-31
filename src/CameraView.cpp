#include "CameraView.h"

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

void CameraView::setup() {
  // setup mesh
  const Trade::MeshData2D square =
      Primitives::Square::solid(Square::TextureCoords::Generate);
  buffer.setData(
      MeshTools::interleave(square.positions(0), square.textureCoords2D(0)),
      BufferUsage::StaticDraw);
  mesh.setPrimitive(square.primitive())
      .setCount(square.positions(0).size())
      .addVertexBuffer(buffer, 0, CameraViewShader::Position{},
                       CameraViewShader::TextureCoordinates{});
}

void CameraView::updateTexture(const FlyCapture2::Image &fcImage) {
  // convert to rgb
  FlyCapture2::Image rgbImage;
  fcImage.Convert(FlyCapture2::PIXEL_FORMAT_RGB, &rgbImage);
  // retrieve image size
  const Vector2i size(rgbImage.GetCols(), rgbImage.GetRows());
  // copy data to magnum raw buffer
  Containers::Array<char> data{rgbImage.GetDataSize()};
  std::copy_n(rgbImage.GetData(), data.size(), data.begin());
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

void CameraView::loadTexture() {
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

void CameraView::draw() {
  shader.setTexture(texture);
  mesh.draw(shader);
}
