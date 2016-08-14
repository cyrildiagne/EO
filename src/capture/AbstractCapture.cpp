#include "capture/AbstractCapture.h"

namespace eo {
namespace capture {

AbstractCapture::AbstractCapture() {}
AbstractCapture::~AbstractCapture() {}

bool AbstractCapture::setup() { return do_setup(); }

void AbstractCapture::update() { do_update(); }

void AbstractCapture::saveImage(float scale) { do_saveImage(scale); }

bool AbstractCapture::hasNewImage() { return do_hasNewImage(); };

const cv::Mat &AbstractCapture::getCvImage() { return do_getCvImage(); }

} // namespace capture
} // namespace eo
