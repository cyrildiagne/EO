#ifndef FlyCaptureCamera_h
#define FlyCaptureCamera_h

#include "flycapture/FlyCapture2.h"
#include <opencv2/core/core.hpp>

#include "capture/AbstractCapture.h"

namespace eo {
namespace capture {

class FlyCaptureCamera : public AbstractCapture {
public:
  FlyCaptureCamera() : isImageNew(false) {}
  virtual ~FlyCaptureCamera();

  // specifics
  bool setProperty(FlyCapture2::PropertyType type, float value);
  bool setWhiteBalance(int red, int blue);

private:
  // interface implementation
  bool do_setup() final;
  void do_update() final;
  void do_saveImage(float scale) final;
  bool do_hasNewImage() final;
  const cv::Mat &do_getCvImage() final;
  // properties
  FlyCapture2::Camera camera;
  FlyCapture2::Image rawImage;
  cv::Mat cvImage;
  bool isImageNew;
  void updateCvImage();
};

} // namespace capture
} // namespace eo

#endif /* end of include guard: FlyCaptureCamera_h */
