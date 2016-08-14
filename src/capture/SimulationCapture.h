#ifndef capture_SimulationCapture_h
#define capture_SimulationCapture_h

#include "capture/AbstractCapture.h"

namespace eo {
namespace capture {

class SynthCircle {
public:
  SynthCircle(cv::Point p, float r, cv::Point s)
      : position(p), radius(r), speed(s) {}
  cv::Point position;
  float radius;
  cv::Point speed;
};

class SimulationCapture : public AbstractCapture {
public:
  static cv::Point mouse;
  SimulationCapture();
  virtual ~SimulationCapture();

private:
  // interface implementation
  bool do_setup() final;
  void do_update() final;
  void do_saveImage(float scale) final;
  bool do_hasNewImage() final;
  const cv::Mat &do_getCvImage() final;

  cv::Mat cvImage;
  bool isImageNew;
  std::vector<SynthCircle> circles;
};

} // namespace eo
} // namespace capture

#endif /* end of include guard: capture_SimulationCapture_h */
