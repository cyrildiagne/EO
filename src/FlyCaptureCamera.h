#ifndef FlyCaptureCamera_h
#define FlyCaptureCamera_h

#include "flycapture/FlyCapture2.h"
#include <opencv2/core/core.hpp>

class FlyCaptureCamera {
public:
  ~FlyCaptureCamera();
  void setup();
  void update();
  void saveImage(float scale);
  cv::Mat getCvMat();
  FlyCapture2::Image rawImage;

private:
  FlyCapture2::Camera camera;
};

#endif /* end of include guard: FlyCaptureCamera_h */
