#ifndef capture_AbstractCapture_h
#define capture_AbstractCapture_h

#include <opencv2/core/core.hpp>

// Interface for all captures
class AbstractCapture {
public:
  virtual ~AbstractCapture();

  // forbid copying
  AbstractCapture(AbstractCapture const &) = delete;
  AbstractCapture &operator=(AbstractCapture const &) = delete;

  // setup capture
  bool setup();
  // update capture
  void update();
  // save an image of the capture
  void saveImage(float scale);
  // inform if new image has been captured during last update
  bool hasNewImage();
  // retrieves the latest image
  const cv::Mat &getCvImage();

protected:
  // allow construction for child classes only
  AbstractCapture();

private:
  virtual bool do_setup() = 0;
  virtual void do_update() = 0;
  virtual void do_saveImage(float scale) = 0;
  virtual bool do_hasNewImage() = 0;
  virtual const cv::Mat &do_getCvImage() = 0;
};

#endif /* end of include guard: capture_AbstractCapture_h */
