
// OTA.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <memory>

// 3rdPartyLibs
#include <glm/glm.hpp>

// Caelum

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes

class OTA
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
  protected:
    double      _aperature;       // Size of the telescope's opening (in mm)
    double      _focalLength;     // Distance from the lens to the focal point (in mm) 
    double      _focalRatio;      // Ratio of focal length to aperature (focalLength / aperature)
    double      _fovDeg;          // field of view in degrees


  public:   

  // Methods
  private:
  protected:
  public:
    const double aperature(void) const
    { return _aperature; }

    const double focalLength(void) const
    { return _focalLength; }

    const double focalRatio(void) const
    { return _focalRatio; }

    const double fovDeg(void) const
    { return _fovDeg; }

    void setFovDeg(const double d) 
    { 
      _fovDeg = d;
    }

    OTA(const double aperature,const double focalLength);
    ~OTA();
};

typedef std::shared_ptr<OTA>   SpOTA;
//-----------------------------------------------------------------------------

