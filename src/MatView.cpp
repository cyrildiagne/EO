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
  std::size_t dataSize = img.total() * img.elemSize();
  Containers::Array<char> data{dataSize};
  std::copy_n(img.ptr(), data.size(), data.begin());
  // retrieve parsed ImageData from flycapture image
  PixelStorage storage;
  numChannels = img.channels();
  PixelFormat format = (numChannels == 1) ? PixelFormat::Red : PixelFormat::RGB;
  Trade::ImageData2D image{storage, format, PixelType::UnsignedByte, size,
                           std::move(data)};
  // Set texture data and parameters
  texture.setWrapping(Sampler::Wrapping::ClampToEdge)
      .setMagnificationFilter(Sampler::Filter::Linear)
      .setMinificationFilter(Sampler::Filter::Linear)
      .setStorage(1, TextureFormat::RGB8, image.size())
      .setSubImage(0, {}, image);
}

void MatView::draw() {
  shader.setTexture(texture);
  shader.setNumChannels(numChannels);
  mesh.draw(shader);
}
