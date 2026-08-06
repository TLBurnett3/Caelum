
// Sensor.cpp 
// Thomas Burnett


//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs
#include <opencv2/opencv.hpp>

// CRos
#include "Sensor.h"

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Sensor
// Pixel scale factor in camera focal length units: f_pixels = sensor_width_pixels / (2 * tan(fov_x / 2))
// Or simplified directly using focalLength / pixelSize ratio:
// xPixel = CenterX + (xTangent * f_in_pixels)
// Note: Assuming f_pixels is pre-derived or transformed via half-width offset.
// Convert normalized projection coordinates to continuous pixel coordinates
// Assuming xTangent and yTangent are angular offsets relative to optical center:
//-----------------------------------------------------------------------------
void Sensor::render(const Star& star, double xTangent, double yTangent,cv::Mat &image)
{
  // Convert Gnomonic Tangent Coordinates (xCam/zCam, yCam/zCam) to Pixel space 
  // Convert normalized projection coordinates to continuous pixel coordinates
  // Assuming xTangent and yTangent are angular offsets relative to optical center:
  double focalLengthPixels = _halfWidth.x / std::tan(glm::radians(_fovDeg.x * 0.5f));

  double xPixel = _halfWidth.x + (xTangent * focalLengthPixels);
  // Y pixel direction is inverted in standard image matrices (0 is top)
  double yPixel = _halfWidth.y - (yTangent * focalLengthPixels);

  // 2. Sensor Boundary Check
  bool visible = (xPixel >= 0.0 && xPixel < static_cast<double>(_imageSize.x) &&
                  yPixel >= 0.0 && yPixel < static_cast<double>(_imageSize.y));

  if (visible)
  {
    // Round to nearest pixel coordinate to avoid systematic 1-px drift
    cv::Point targetPoint(static_cast<int>(std::round(xPixel)), 
                          static_cast<int>(std::round(yPixel)));

    // Draw star centroid on the sensor frame
    cv::circle(image, targetPoint, 1, cv::Scalar(255, 255, 255), -1);
  }
}


//-----------------------------------------------------------------------------
// Sensor
//-----------------------------------------------------------------------------
Sensor::Sensor(const glm::vec2 sensorSize,const float  pixelSize,
               const float focalLength)                             : _sensorSize(sensorSize), 
                                                                      _pixelSize(pixelSize),
                                                                      _imageSize(static_cast<int>(_sensorSize.x / _pixelSize),
                                                                                 static_cast<int>(_sensorSize.y / _pixelSize)),
                                                                      _halfWidth(_imageSize * 0.5f)
{
  _fovDeg.x = (2.0f * glm::degrees(atan((_sensorSize.x / 2.0f) / focalLength)));
  _fovDeg.y = (2.0f * glm::degrees(atan((_sensorSize.y / 2.0f) / focalLength)));
}

//-----------------------------------------------------------------------------
// ~Sensor
//-----------------------------------------------------------------------------
Sensor::~Sensor()
{

}

