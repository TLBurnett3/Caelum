
// ColorLUT.cpp 
// Thomas Burnett


//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// CRos
#include "ColorLUT.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ColorLUT
//-----------------------------------------------------------------------------
cv::Scalar ColorLUT::bvToBGR(double bv) 
{
  // Clamp B-V to realistic astronomical bounds [-0.4, 2.0]
  bv = std::clamp(bv, -0.4, 2.0);

  // 1. Ballesteros' Formula: B-V to Temperature (Kelvin)
  double temp = 4600.0 * ( (1.0 / (0.92 * bv + 1.7)) + (1.0 / (0.92 * bv + 0.62)) );

  // Clamp temperature for RGB algorithm lookup range [1000K, 40000K]
  temp = std::clamp(temp, 1000.0, 40000.0) / 100.0;

  double r = 0.0, g = 0.0, b = 0.0;

  // 2. Calculate Red
  if (temp <= 66.0) {
      r = 255.0;
  } else {
      r = temp - 60.0;
      r = 329.698727446 * std::pow(r, -0.1332047592);
      r = std::clamp(r, 0.0, 255.0);
  }

  // 3. Calculate Green
  if (temp <= 66.0) {
      g = temp;
      g = 99.4708025861 * std::log(g) - 161.1195681661;
      g = std::clamp(g, 0.0, 255.0);
  } else {
      g = temp - 60.0;
      g = 288.1221695283 * std::pow(g, -0.0755148492);
      g = std::clamp(g, 0.0, 255.0);
  }

  // 4. Calculate Blue
  if (temp >= 66.0) {
      b = 255.0;
  } else if (temp <= 19.0) {
      b = 0.0;
  } else {
      b = temp - 10.0;
      b = 138.5177312231 * std::log(b) - 305.0447927307;
      b = std::clamp(b, 0.0, 255.0);
  }

  // Return as OpenCV BGR (Blue, Green, Red)
  return cv::Scalar(
      static_cast<uchar>(std::round(b)),
      static_cast<uchar>(std::round(g)),
      static_cast<uchar>(std::round(r)));
}




//-----------------------------------------------------------------------------
// ColorLUT
//-----------------------------------------------------------------------------
ColorLUT::ColorLUT(void) : _lut(241)
{
  for (int i = 0; i <= 240; ++i) 
  {
  double bv = -0.40 + (i * 0.01);

    _lut[i] = bvToBGR(bv);
  }
}

//-----------------------------------------------------------------------------
// ~ColorLUT
//-----------------------------------------------------------------------------
ColorLUT::~ColorLUT()
{

}

