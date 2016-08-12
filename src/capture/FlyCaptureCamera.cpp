#include "capture/FlyCaptureCamera.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

FlyCaptureCamera::~FlyCaptureCamera() {
  if (!camera.IsConnected()) {
    return;
  }
  std::cout << "disconnecting camera.." << std::endl;
  FlyCapture2::Error error = camera.StopCapture();
  if (error != FlyCapture2::PGRERROR_OK) {
    // This may fail when the camera was removed, so don't show
    // an error message
  }
  camera.Disconnect();
  std::cout << "Camera disconnected." << std::endl;
}

bool FlyCaptureCamera::setup() {
  FlyCapture2::Error error;
  // Connect the camera
  error = camera.Connect();
  if (error != FlyCapture2::PGRERROR_OK) {
    std::cout << "Failed to connect to camera" << std::endl;
    return false;
  }
  // Get the camera info and print it out
  FlyCapture2::CameraInfo camInfo;
  error = camera.GetCameraInfo(&camInfo);
  if (error != FlyCapture2::PGRERROR_OK) {
    std::cout << "Failed to get camera info from camera" << std::endl;
    return false;
  }
  std::cout << camInfo.vendorName << " " << camInfo.modelName << " "
            << camInfo.serialNumber << std::endl;
  // Start capture
  error = camera.StartCapture();
  if (error == FlyCapture2::PGRERROR_ISOCH_BANDWIDTH_EXCEEDED) {
    std::cout << "Bandwidth exceeded" << std::endl;
    return false;
  } else if (error != FlyCapture2::PGRERROR_OK) {
    std::cout << "Failed to start image capture" << std::endl;
    return false;
  }
  return true;
}

void FlyCaptureCamera::update() {
  isImageNew = false;
  FlyCapture2::Error error = camera.RetrieveBuffer(&rawImage);
  if (error != FlyCapture2::PGRERROR_OK) {
    // std::cout << "capture error" << std::endl;
  } else {
    updateCvImage();
    isImageNew = true;
  }
}

void FlyCaptureCamera::saveImage(float scale) {
  cv::Mat resizedFrame;
  resizedFrame.create(cvImage.rows * scale, cvImage.cols * scale, CV_8UC3);
  cv::resize(cvImage, resizedFrame, resizedFrame.size(), 0, 0, cv::INTER_AREA);
  // retrieve current time
  time_t now = time(NULL);
  struct tm tstruct;
  char buf[40];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%H-%M-%S", &tstruct);
  cv::imwrite("export/" + std::string{buf} + ".jpg", resizedFrame);
}

void FlyCaptureCamera::updateCvImage() {
  // convert to rgb
  FlyCapture2::Image rgbImage;
  rawImage.Convert(FlyCapture2::PIXEL_FORMAT_RGB, &rgbImage);
  // convert to OpenCV Mat
  unsigned int rowBytes = static_cast<double>(rgbImage.GetReceivedDataSize()) /
                          static_cast<double>(rgbImage.GetRows());
  // TODO: find why we get artefacts when we remove the clone
  cvImage = cv::Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3,
                    rgbImage.GetData(), rowBytes)
                .clone();
}
