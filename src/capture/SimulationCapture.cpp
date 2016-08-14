#include "capture/SimulationCapture.h"

#include <iostream>
#include <random>

#include <opencv2/imgproc/imgproc.hpp>

namespace eo {
namespace capture {

cv::Point SimulationCapture::mouse;

SimulationCapture::SimulationCapture() : isImageNew(false) {}
SimulationCapture::~SimulationCapture() {}

bool SimulationCapture::do_setup() {
  cvImage = cv::Mat::zeros(1024, 1280, CV_8UC3);
  // prepare random distribution
  std::random_device rd;
  std::mt19937 d(rd());
  std::uniform_real_distribution<> r(0, 1);
  //
  cv::Point center(cvImage.rows * 0.5, cvImage.cols * 0.5);
  for (int i = 0; i < 5; i++) {
    float radius = r(d) * 60 + 30;
    float sx = r(d) * 20 - 10;
    float sy = r(d) * 20 - 10;
    circles.push_back({center, radius, cv::Point2f{sy, sx}});
  }
  return true;
}

void SimulationCapture::do_update() {
  // clear black
  cvImage.setTo(cv::Scalar(0, 0, 0));
  // draw circles
  int w = cvImage.cols;
  int h = cvImage.rows;
  for (size_t i = 0; i < circles.size(); i++) {
    SynthCircle &c = circles[i];
    c.position.y += c.speed.y;
    c.position.x += c.speed.x;
    if (c.position.y < 0 || c.position.y > h) {
      c.speed.y *= -1;
    }
    if (c.position.x < 0 || c.position.x > w) {
      c.speed.x *= -1;
    }
    cv::circle(cvImage, c.position, c.radius, cv::Scalar(125, 255, 80), -1);
  }
  // draw mouse circle
  cv::circle(cvImage, mouse, 50, cv::Scalar(105, 235, 60), -1);
  isImageNew = true;
}

void SimulationCapture::do_saveImage(float scale) {
  static_cast<void>(scale);
  std::cout << "saveImage not supported on synthetic capture;" << std::endl;
}

bool SimulationCapture::do_hasNewImage() { return isImageNew; };

const cv::Mat &SimulationCapture::do_getCvImage() { return cvImage; }

} // namespace capture
} // namespace eo
