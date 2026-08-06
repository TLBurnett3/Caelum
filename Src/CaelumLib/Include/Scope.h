
// Scope.h
// Thomas Burnett

#pragma once

//---------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs
#include <opencv2/opencv.hpp>

// Caelum
#include "Star.h"
#include "Tracker.h"
#include "OTA.h"
#include "Sensor.h"
#include "Catalog.h"
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// Classes
    struct AltAz {
        double altDeg; // Altitude above horizon (0° to 90°)
        double azDeg;  // Azimuth from North (0° = N, 90° = E, 180° = S, 270° = W)
        bool isVisible;
    };
class Scope
{
  // Defines
  private:
  protected:


  public:

  // Members
  private:
    SpTracker _spTracker;       // Pointer to the Tracker object
    SpOTA     _spOTA;           // Pointer to the OTA object
    SpSensor  _spSensor;        // Pointer to the Sensor object

    float      _longitude;       
    float      _latitude;       
    float      _lst;             // localside real time
    glm::vec3  _direction;       // Unit vector pointing along optical axis
    float      _aperature;       // Size of the telescope's opening (in mm)
    float      _focalLength;     // Distance from the lens to the focal point (in mm) 
    glm::vec2  _sensorSize;      // Size of the camera sensor (in mm)
    float      _pixelSize;       // Size of a single pixel on the sensor (in mm)
    glm::ivec2 _imageSize;       // Size of the image in pixels (width, height)
    float      _focalRatio;      // Ratio of focal length to aperature (focalLength / aperature)
    float      _fovYDeg;         // Angular extent of the observable sky (in degrees)
    float      _cosThreshold;

  protected:
  public:   

  // Methods
  private:
    glm::vec3 altAzToDirection(double altDeg, double azDeg)
    {
    float altRad = glm::radians(static_cast<float>(altDeg));
    float azRad  = glm::radians(static_cast<float>(azDeg));
    float cosAlt = std::cos(altRad);

        return glm::vec3(
            cosAlt * std::sin(azRad), // X = East
            cosAlt * std::cos(azRad), // Y = North
            std::sin(altRad)          // Z = Zenith (Up)
        );
    }

    glm::vec3 celestialToEuclidean(const Star &star) const
    {
      // 1. Convert degrees to radians
      float raRad  = glm::radians(static_cast<float>(star._ra));
      float decRad = glm::radians(static_cast<float>(star._dec));
      float cosDec = std::cos(decRad);

      // Explicit 3D conversion matching astronomical Cartesian conventions
      return glm::vec3(
        cosDec * std::cos(raRad),  // X
        cosDec * std::sin(raRad),  // Y
        std::sin(decRad));         // Z
    }  

    glm::vec3 celestialToEuclidean(double raDeg,double decDeg) const
    {
      // 1. Convert degrees to radians
      float raRad  = glm::radians(static_cast<float>(raDeg));
      float decRad = glm::radians(static_cast<float>(decDeg));
      float cosDec = std::cos(decRad);

      // Explicit 3D conversion matching astronomical Cartesian conventions
      return glm::vec3(
        cosDec * std::cos(raRad),  // X
        cosDec * std::sin(raRad),  // Y
        std::sin(decRad));         // Z
    }  


    // Wrap angle to [0, 360) degrees
    inline double normalize360(double angle) {
      double res = std::fmod(angle, 360.0);
      return (res < 0.0) ? res + 360.0 : res;

    }

  protected:
    AltAz  calculateAltAz(double raDeg, double decDeg, double lstDeg, double latDeg);

  public:
    void projectToImage(const StarField &starField, cv::Mat &image);

    void pointAt(double gmstDegrees, double raT, double decT);

    int track(Catalog &catalog, 
              const char *pStr, const double ra, const double dec,
              const double gmstDegrees, const double tDelta,
              const uint32_t nFrames,ImageLst &imageLst);

    bool isVisible(const Star& star) const;

  
    Scope(SpTracker& spTracker, SpOTA& spOTA, SpSensor& spSensor);


    Scope(const float ra, const float dec, const float aperature, 
          const float focalLength, const glm::vec2 sensorSize,
          const float  pixelSize, const float toleranceDeg = 0.5f);

    ~Scope();
};

typedef std::shared_ptr<Scope>   SpScope;
//-----------------------------------------------------------------------------


