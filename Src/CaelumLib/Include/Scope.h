
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

  protected:
  public:   

  // Methods
  private:
    // ra  = az = long
    // alt = dec = lat 
    glm::vec3 altAzToEuclidean(double azDeg,double altDeg) const
    {
    float altRad = glm::radians(static_cast<float>(altDeg));
    float azRad  = glm::radians(static_cast<float>(azDeg));
    float cosAlt = std::cos(altRad);
    glm::vec3 v =  glm::vec3(
            cosAlt * std::sin(azRad), // X = East
            cosAlt * std::cos(azRad), // Y = North
            std::sin(altRad)          // Z = Zenith (Up)
        );

      return glm::normalize(v);
    }

    glm::vec3 celestialToEuclidean(const Star &star) const
    {  return altAzToEuclidean(star._ra,star._dec); }  

    glm::vec3 celestialToEuclidean(double raDeg,double decDeg) const
    { return altAzToEuclidean(raDeg,decDeg); }  

    glm::vec3 geographicToEuclidean(double longDeg,double latDeg) const
    { return altAzToEuclidean(longDeg,latDeg); }  

    // Wrap angle to [0, 360) degrees
    inline double normalize360(double angle) {
      double res = std::fmod(angle, 360.0);
      return (res < 0.0) ? res + 360.0 : res;
    }

  protected:

  public:
    int track(Catalog &catalog, 
              const char *pStr, const double ra, const double dec,
              const double gmstDegrees, const double tDelta,
              const uint32_t nFrames,ImageLst &imageLst);

    int capture(Catalog& catalog,const char *pTargetName, 
                const double ra, const double dec,
                const double currentGMST, const double exposureTime, 
                const int numFrames);

    int Scope::capture(Catalog& catalog, const char *pTargetName, 
             const double currentGMST, const double exposureTime, 
             const int numFrames);
 
    Scope(SpTracker& spTracker, SpOTA& spOTA, SpSensor& spSensor);

    ~Scope();
};

typedef std::shared_ptr<Scope>   SpScope;
//-----------------------------------------------------------------------------


