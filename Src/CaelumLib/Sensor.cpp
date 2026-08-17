
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
void Sensor::render(const Star& star, const double exposureTime,
                    double xTangent, double yTangent,
                    cv::Mat &image)
{
// 1. Convert Gnomonic Tangent Coordinates to Continuous Pixel Coordinates
  double focalLengthPixels = _halfWidth.x / std::tan(glm::radians(_fovDeg.x * 0.5f));

  double xPixel = _halfWidth.x + (xTangent * focalLengthPixels);
  double yPixel = _halfWidth.y - (yTangent * focalLengthPixels);

  // Quick bounding box check (including PSF radius allowance)
  float sigma = _fwhmPixels / 2.35482f;
  int radius = static_cast<int>(std::ceil(sigma * 4.0f));

  if (xPixel < -radius || xPixel >= image.cols + radius ||
      yPixel < -radius || yPixel >= image.rows + radius)
  {
    return; // Fast reject off-sensor stars
  }

  // 2. Photometric Energy Calculation
  // 0-mag Vega baseline ~1e6 photoelectrons/sec/cm^2 (adjusted by aperture/aperture efficiency)
  float fluxRate = 1.0e6f * std::pow(10.0f, -0.4f * static_cast<float>(star.visualMagnitude())); 
  float totalElectrons = fluxRate * static_cast<float>(exposureTime);

  // Early cut-off for undetectable sub-electron flux
  if (totalElectrons < 1e-4f) return; 

  cv::Vec3f bgrWeights = getBGRWeightsFromBV(star.bvColorIndex());
  cv::Vec3f totalChannelElectrons = bgrWeights * totalElectrons;

  // 3. PSF Gaussian Splat
  int minX = std::max(0, static_cast<int>(std::floor(xPixel - radius)));
  int maxX = std::min(image.cols - 1, static_cast<int>(std::ceil(xPixel + radius)));
  int minY = std::max(0, static_cast<int>(std::floor(yPixel - radius)));
  int maxY = std::min(image.rows - 1, static_cast<int>(std::ceil(yPixel + radius)));

  float twoSigmaSq = 2.0f * sigma * sigma;
  float normFactor = 1.0f / (3.14159265358979323846f * twoSigmaSq);
  cv::Vec3f psfAmplitude = totalChannelElectrons * normFactor;

  for (int y = minY; y <= maxY; ++y) 
  {
    cv::Vec3f* rowPtr = image.ptr<cv::Vec3f>(y);
    float dy = static_cast<float>(y) - static_cast<float>(yPixel);
    float dySq = dy * dy;

    for (int x = minX; x <= maxX; ++x) 
    {
      float dx = static_cast<float>(x) - static_cast<float>(xPixel);
      float distSq = dx * dx + dySq;

      // 2D Gaussian Kernel
      float psfIntensity = std::exp(-distSq / twoSigmaSq);

      // Accumulate photoelectrons into 32-bit float BGR matrix
      rowPtr[x] += psfAmplitude * psfIntensity;
    }
  }
}


//-----------------------------------------------------------------------------
// Sensor
//-----------------------------------------------------------------------------
Sensor::Sensor(const SpOTA &spOTA,const glm::vec2 sensorSize,const float  pixelSize) : _sensorSize(sensorSize), 
                                                                      _pixelSize(pixelSize),
                                                                      _imageSize(static_cast<int>(_sensorSize.x / _pixelSize),
                                                                                 static_cast<int>(_sensorSize.y / _pixelSize)),
                                                                      _halfWidth(_imageSize * 0.5f),
                                                                      _fovDeg(0.0f, 0.0f),
                                                                      _colorLUT(),
                                                                      _fwhmPixels(0.0),
                                                                      _sigma(0.0)                                         
{
// Defined for smart telescope system:
double seeingArcsec = 2.5f;          // Atmospheric seeing condition
// Compute pixel scale (arcsec / pixel)
double pixelScale = (206.265 * (_pixelSize * 1000.0)) / spOTA->focalLength(); // ~3.10 arcsec/pixel
// Convert seeing to pixels
double fwhmSeeingPix = seeingArcsec / pixelScale; // ~0.80 pixels
// Compute optical diffraction limit in pixels
double fwhmOpticsArcsec = 0.21f * (550.0f / spOTA->aperature()); // ~2.31 arcsec
double fwhmOpticsPix = fwhmOpticsArcsec / pixelScale;    // ~0.74 pixels

  _fovDeg.x = (2.0f * glm::degrees(atan((_sensorSize.x / 2.0f) / spOTA->focalLength())));
  _fovDeg.y = (2.0f * glm::degrees(atan((_sensorSize.y / 2.0f) / spOTA->focalLength())));

  spOTA->setFovDeg(std::sqrt(std::pow(_fovDeg.x,2) + std::pow(_fovDeg.y,2)));

  // Root-sum-square total FWHM in pixels
  _fwhmPixels = std::sqrt(fwhmSeeingPix * fwhmSeeingPix + fwhmOpticsPix * fwhmOpticsPix);

  // Convert FWHM to Gaussian Sigma for rendering
  _sigma = _fwhmPixels / 2.35482f;
}

//-----------------------------------------------------------------------------
// ~Sensor
//-----------------------------------------------------------------------------
Sensor::~Sensor()
{

}

