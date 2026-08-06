
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

    glm::vec2  _fovDeg;
  public:   

  // Methods
  private:
  protected:
  public:
    uint32_t imageWidth(void)
    { return _imageSize.x; }

    uint32_t imageHeight(void)
    { return _imageSize.y; }

    void Sensor::render(const Star& star, double xTangent, double yTangent,cv::Mat &image);

    Sensor(const glm::vec2 sensorSize,const float  pixelSize,const float focalLength);
    ~Sensor();
};

typedef std::shared_ptr<Sensor>   SpSensor;
typedef std::vector<cv::Mat>      ImageLst;

//-----------------------------------------------------------------------------

