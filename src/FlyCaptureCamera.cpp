#include "FlyCaptureCamera.h"
#include <iostream>

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

void FlyCaptureCamera::setup() {
  FlyCapture2::Error error;
  // Connect the camera
  error = camera.Connect();
  if (error != FlyCapture2::PGRERROR_OK) {
    std::cout << "Failed to connect to camera" << std::endl;
    return;
  }
  // Get the camera info and print it out
  FlyCapture2::CameraInfo camInfo;
  error = camera.GetCameraInfo(&camInfo);
  if (error != FlyCapture2::PGRERROR_OK) {
    std::cout << "Failed to get camera info from camera" << std::endl;
    return;
  }
  std::cout << camInfo.vendorName << " " << camInfo.modelName << " "
            << camInfo.serialNumber << std::endl;
  // Start capture
  error = camera.StartCapture();
  if (error == FlyCapture2::PGRERROR_ISOCH_BANDWIDTH_EXCEEDED) {
    std::cout << "Bandwidth exceeded" << std::endl;
    return;
  } else if (error != FlyCapture2::PGRERROR_OK) {
    std::cout << "Failed to start image capture" << std::endl;
    return;
  }
}

void FlyCaptureCamera::update() {
  FlyCapture2::Error error = camera.RetrieveBuffer(&rawImage);
  if (error != FlyCapture2::PGRERROR_OK) {
    // std::cout << "capture error" << std::endl;
  }
}

cv::Mat FlyCaptureCamera::getCvMat() {
  // convert to rgb
  FlyCapture2::Image rgbImage;
  rawImage.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage);
  // convert to OpenCV Mat
  unsigned int rowBytes = static_cast<double>(rgbImage.GetReceivedDataSize()) /
                          static_cast<double>(rgbImage.GetRows());
  cv::Mat image = cv::Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3,
                          rgbImage.GetData(), rowBytes);
  return image;
}
