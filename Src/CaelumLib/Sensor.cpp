
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
                    cv::Mat &iPrime,cv::Mat &iLight)
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

    // Draw star centroid on the RGB sensor frame
    cv::circle(iPrime, targetPoint, 1, _colorLUT.getColor(star._bvColorIndex), -1);

    {
    cv::circle(iLight, targetPoint, 1, _colorLUT.getColor(star._bvColorIndex), -1);
    }


    {
    // Total photon flux rate (e-/sec across the optical bandpass)
    // Assumes Phi0 (0-mag flux) = ~1000 photons/sec/cm^2/A
    float fluxRate = 1.0e6f * std::pow(10.0f, -0.4f * star._magnitude); 
    float totalElectrons = fluxRate * exposureTime;

    // Determine channel distribution
    cv::Vec3f bgrWeights = getBGRWeightsFromBV(star._bvColorIndex);
    cv::Vec3f totalChannelElectrons = bgrWeights * totalElectrons;

    // PSF Geometry (2D Gaussian)
    float sigma = _fwhmPixels / 2.35482f; // Convert FWHM to sigma
    int radius = static_cast<int>(std::ceil(sigma * 4.0f)); // 4-sigma bounding box

    int minX = std::max(0, static_cast<int>(std::floor(xPixel - radius)));
    int maxX = std::min(iLight.cols - 1, static_cast<int>(std::ceil(xPixel + radius)));
    int minY = std::max(0, static_cast<int>(std::floor(yPixel - radius)));
    int maxY = std::min(iLight.rows - 1, static_cast<int>(std::ceil(yPixel + radius)));

    float twoSigmaSq = 2.0f * sigma * sigma;
    float normFactor = 1.0f / (3.14159265f * twoSigmaSq);

      // Splat Gaussian onto the 3-channel 32-bit floating point matrix
      for (int y = minY; y <= maxY; ++y) 
      {
      cv::Vec3f* rowPtr = iLight.ptr<cv::Vec3f>(y);
      float dy = y - yPixel;
      float dySq = dy * dy;

        for (int x = minX; x <= maxX; ++x) 
        {
        float dx = x - xPixel;
        float distSq = dx * dx + dySq;
        // Normalized 2D Gaussian spatial fraction
        float psfIntensity = normFactor * std::exp(-distSq / twoSigmaSq);

          // Add scaled photoelectrons directly to B, G, and R channels
          rowPtr[x] += totalChannelElectrons * psfIntensity;
        }
      }
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

