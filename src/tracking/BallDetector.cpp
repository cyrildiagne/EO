#include "BallDetector.h"

#include <opencv2/imgproc/imgproc.hpp>

namespace eo {
namespace tracking {

BallDetector::BallDetector()
    : rescale(0.50), minHue(50), maxHue(100), minSaturation(60),
      maxSaturation(250), minValue(30), maxValue(150) {}

void BallDetector::process(const cv::Mat &frame, bool bDebug) {
  // get scaled down copy
  if (resizedFrame.rows != frame.rows || resizedFrame.cols != frame.cols) {
    resizedFrame.create(frame.rows * rescale, frame.cols * rescale, CV_8UC3);
  }
  cv::resize(frame, resizedFrame, resizedFrame.size(), 0, 0, cv::INTER_LINEAR);
  // convert to hsv
  cv::cvtColor(resizedFrame, hsvFrame, CV_BGR2HSV);
  // threshold
  cv::Scalar hsv_min = cv::Scalar(minHue, minSaturation, minValue);
  cv::Scalar hsv_max = cv::Scalar(maxHue, maxSaturation, maxValue);
  cv::inRange(hsvFrame, hsv_min, hsv_max, threshFrame);
  // erode / dilate morph
  const int morphSize = 2;
  const int morph_elem = 2; // 0: Rect - 1: Cross - 2: Ellipse
  float msize = morphSize;
  const cv::Point pos = cv::Point(msize, msize);
  // blur
  cv::GaussianBlur(threshFrame, morphFrame, cv::Size(3, 3), 2, 2);
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

  // compute hough circles as backup for overlapping balls
  std::vector<cv::Vec3f> hghCircles;
  /*
  cv::GaussianBlur(morphFrame, morphFrame, cv::Size(3, 3), 2, 2);
  cv::Mat hghimg = morphFrame.clone();
  cv::HoughCircles(hghimg, hghCircles, CV_HOUGH_GRADIENT, 1, 10, 200, 20, 0, 0);
  if (bDebug) {
    for (size_t i = 0; i < hghCircles.size(); i++) {
      cv::circle(frame, cv::Point(hghCircles[i][0], hghCircles[i][1]) / rescale,
                 hghCircles[i][2] / rescale, cv::Scalar(0, 0, 255), 3);
    }
  }
  */

  // find
  circles.clear();
  int total = contours.size();
  for (int i = 0; i < total; i++) {
    // get enclosing circle
    float radius;
    cv::Point2f center;
    cv::minEnclosingCircle(static_cast<cv::Mat>(contours[i]), center, radius);
    if (radius < 5) {
      continue;
    } // otherwise compare radius to ball area
    // else if (3.14159 * radius * radius > cv::contourArea(contours[i]) * 1.75)
    // {
    //   // look for hough circles inside the minEnclosingCircle
    //   for (size_t j = 0; j < hghCircles.size(); j++) {
    //     const float hghX = hghCircles[i][0];
    //     const float hghY = hghCircles[i][1];
    //     const float hghRadius = hghCircles[i][2];
    //     const float dx = hghX - center.x;
    //     const float dy = hghY - center.y;
    //     const float dist = sqrt(dx * dx + dy * dy);
    //     if (dist < radius - hghRadius + 20) {
    //       cv::circle(frame, cv::Point(hghX, hghY) / rescale,
    //                  hghRadius / rescale, cv::Scalar(0, 255, 0), 1);
    //       float cx = hghX - resizedFrame.cols * 0.5;
    //       float cy = hghY - resizedFrame.rows * 0.5;
    //       circles.push_back(
    //           DetectedCircle{-cx / rescale, cy / rescale, hghRadius /
    //           rescale});
    //     }
    //   }
    //   if (bDebug) {
    //     cv::circle(frame, center / rescale, radius / rescale,
    //                cv::Scalar(255, 0, 0), 1);
    //   }
    //   continue;
    // }
    if (bDebug) {
      cv::circle(frame, center / rescale, radius / rescale,
                 cv::Scalar(0, 255, 0), 1);
    }

    float cx = center.x - resizedFrame.cols * 0.5;
    float cy = center.y - resizedFrame.rows * 0.5;
    circles.push_back(
        DetectedCircle{-cx / rescale, cy / rescale, radius / rescale});
  }
}

} // namespace eo
} // namespace tracking
