
// OTA.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <memory>
// 3rdPartyLibs

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

  public:   

  // Methods
  private:
  protected:
  public:
  
    void pointAt(double ra, double dec);

    OTA(const double aperature,const double focalLength);
    ~OTA();
};

typedef std::shared_ptr<OTA>   SpOTA;
//-----------------------------------------------------------------------------

