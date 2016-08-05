#include "BallTracker.h"
#include <chrono>

void BallTracker::setup() {}

const cv::Mat &BallTracker::getImage() { return currFrame; }

void BallTracker::update(const cv::Mat frame) {
  // start chrono
  auto start = std::chrono::steady_clock::now();
  // get scaled down copy
  const float scale = 0.25;
  cv::Mat resizedFrame;
  resizedFrame.create(frame.rows * scale, frame.cols * scale, CV_8UC3);
  cv::resize(frame, resizedFrame, resizedFrame.size(), 0, 0, cv::INTER_LINEAR);
  // convert to hsv
  cv::Mat hsvFrame;
  cv::cvtColor(resizedFrame, hsvFrame, CV_BGR2HSV);
  // threshold
  const int minHue = 5;
  const int maxHue = 120;
  const int minSaturation = 0;
  const int maxSaturation = 255;
  const int minValue = 27;
  const int maxValue = 255;
  cv::Scalar hsv_min = cv::Scalar(minHue, minSaturation, minValue);
  cv::Scalar hsv_max = cv::Scalar(maxHue, maxSaturation, maxValue);
  cv::Mat threshFrame;
  cv::inRange(hsvFrame, hsv_min, hsv_max, threshFrame);
  // erode / dilate morph
  cv::Mat morphFrame;
  const int morphSize = 6;
  const int morph_elem = 2; // 0: Rect - 1: Cross - 2: Ellipse
  float msize = morphSize;
  const cv::Point pos = cv::Point(msize, msize);
  // erode
  cv::Mat element1 = getStructuringElement(
      morph_elem, cv::Size(2 * msize + 1, 2 * msize + 1), pos);
  cv::morphologyEx(threshFrame, morphFrame, cv::MORPH_OPEN, element1);
  // dilate
  cv::Mat element2 = getStructuringElement(
      morph_elem, cv::Size(2 * msize + 1, 2 * msize + 1), pos);
  cv::morphologyEx(morphFrame, morphFrame, cv::MORPH_CLOSE, element2);
  // find contours
  typedef std::vector<cv::Point> PointVec;
  std::vector<PointVec> contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(morphFrame.clone(), contours, hierarchy, CV_RETR_EXTERNAL,
                   CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
  // find
  circles.clear();
  int total = contours.size();
  for (int i = 0; i < total; i++) {
    // get enclosing circle
    float radius;
    cv::Point2f center;
    cv::minEnclosingCircle(static_cast<cv::Mat>(contours[i]), center, radius);
    if (radius < 10) {
      continue;
    }
    float cx = center.x - resizedFrame.cols * 0.5;
    float cy = center.y - resizedFrame.rows * 0.5;
    circles.push_back(Circle{-cx / scale, cy / scale, radius / scale});
  }
  // update chrono
  auto end = std::chrono::steady_clock::now();
  auto diff = end - start;
  trackTime = std::chrono::duration<double, std::milli>(diff).count();
  // update currframe for rendering
  currFrame = resizedFrame;
}
