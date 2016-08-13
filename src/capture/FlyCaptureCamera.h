#ifndef FlyCaptureCamera_h
#define FlyCaptureCamera_h

#include "flycapture/FlyCapture2.h"
#include <opencv2/core/core.hpp>

class FlyCaptureCamera {
public:
  FlyCaptureCamera() : isImageNew(false) {}
  ~FlyCaptureCamera();
  bool setup();
  void update();
  bool setProperty(FlyCapture2::PropertyType type, float value);
  bool setWhiteBalance(int red, int blue);
  void saveImage(float scale);
  bool hasNewImage() { return isImageNew; };
  const cv::Mat &getCvImage() { return cvImage; }

private:
  FlyCapture2::Camera camera;
  FlyCapture2::Image rawImage;
  cv::Mat cvImage;
  bool isImageNew;
  void updateCvImage();
};

#endif /* end of include guard: FlyCaptureCamera_h */
