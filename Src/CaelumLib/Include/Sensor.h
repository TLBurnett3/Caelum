
// Sensor.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <memory>

// 3rdPartyLibs
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>

// Caelum
#include "Star.h"
#include "ColorLUT.h"
#include "OTA.h"
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes

class Sensor
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
  protected:
    glm::vec2   _sensorSize;
    float       _pixelSize; 
    glm::vec2   _imageSize;
    glm::vec2   _halfWidth;

    glm::vec2   _fovDeg;

    ColorLUT    _colorLUT;

    double      _fwhmPixels;
    double      _sigma;

  public:   

  // Methods
  private:
  protected:
  public:
    uint32_t imageWidth(void)
    { return _imageSize.x; }

    uint32_t imageHeight(void)
    { return _imageSize.y; }

    

    // Helper to estimate normalized BGR flux weights from star's B-V index
    cv::Vec3f getBGRWeightsFromBV(float bv) 
    {
      // Clamped B-V range approx [-0.4 (Hot Blue) to 2.0 (Cool Red)]
      bv = std::clamp(bv, -0.4f, 2.0f);
    
      // Simple piecewise or polynomial approximation for visual BGR response:
      float r = std::clamp(0.2f + 0.5f * (bv + 0.4f), 0.1f, 0.8f);
      float b = std::clamp(0.8f - 0.5f * (bv + 0.4f), 0.1f, 0.8f);
      float g = 1.0f - (r + b); // Keep total normalized sum = 1.0
    
      if (g < 0.1f) g = 0.1f;
      float norm = r + g + b;
    
      // OpenCV uses BGR channel ordering
      return cv::Vec3f(b / norm, g / norm, r / norm); 
    }

    void render(const Star& star,const double exposureTime, double xTangent, double yTangent,cv::Mat &image);

    Sensor(const SpOTA &spOTA,const glm::vec2 sensorSize,const float  pixelSize);
    ~Sensor();
};

typedef std::shared_ptr<Sensor>   SpSensor;
typedef std::vector<cv::Mat>      ImageLst;
//-----------------------------------------------------------------------------

