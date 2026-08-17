
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

class Scope
{
  // Defines
  private:
    struct BinSpan
    {
      int min_ra, max_ra; // [0..359]
    };

    struct BinSearchSpace 
    {
      int min_dec_bin;    // [0..179]
      int max_dec_bin;    // [0..179]
    
      // RA spans per declination row (handles RA wraparound and polar expansion)
      // Most rows have 1 span. Crossed 0/360 boundary rows have 2 spans.
      std::vector<std::pair<int, std::vector<BinSpan>>> dec_row_spans; 
    
      bool touches_north_pole = false;
      bool touches_south_pole = false;
    };
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
    {  return altAzToEuclidean(star.raDeg(),star.decDeg()); }  

    glm::vec3 celestialToEuclidean(double raDeg,double decDeg) const
    { return altAzToEuclidean(raDeg,decDeg); }  

    glm::vec3 geographicToEuclidean(double longDeg,double latDeg) const
    { return altAzToEuclidean(longDeg,latDeg); }  

    // Wrap angle to [0, 360) degrees
    inline double normalize360(double angle) {
      double res = std::fmod(angle, 360.0);
      return (res < 0.0) ? res + 360.0 : res;
    }

    inline void unpackTychoID(uint32_t id, uint16_t *tyc1, uint16_t *tyc2, uint16_t *tyc3) 
    {
      *tyc1 = static_cast<uint16_t>((id >> 17) & 0x3FFF); // Top 14 bits
      *tyc2 = static_cast<uint16_t>((id >> 3)  & 0x3FFF); // Middle 14 bits
      *tyc3 = static_cast<uint16_t>(id & 0x07);           // Bottom 3 bits
    }

    void Scope::getBinSearchSpace(BinSearchSpace &space,
                                  const double center_ra, const double center_dec, 
                                  const double fov_radius_deg);

  protected:
    cv::Mat visualizeStellarFlux(const cv::Mat& inputImg, bool applyNonLinearStretch = true, float stretchFactor = 100.0f); 
    cv::Mat visualizeStellarFlux(const cv::Mat& rawSensorImage, float stretchScale);

  public:
    int track(Catalog &catalog, 
              const char *pStr, const double ra, const double dec,
              const double gmstDegrees, const double tDelta,
              const uint32_t nFrames,ImageLst &imageLst,const bool display = false);

    int captureImageSet(Catalog& catalog,const char *pTargetName, 
                const double ra, const double dec,
                const double currentGMST, const double exposureTime, 
                const int numFrames,const bool display = false);

    int captureImageSet(Catalog& catalog, const char *pTargetName, 
             const double currentGMST, const double exposureTime, 
             const int numFrames,const bool display = false);
 

 
    Scope(SpTracker& spTracker, SpOTA& spOTA, SpSensor& spSensor);

    ~Scope();
};

typedef std::shared_ptr<Scope>   SpScope;
//-----------------------------------------------------------------------------


