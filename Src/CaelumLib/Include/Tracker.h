
// Tracker.h
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

class Tracker
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
  protected:
    double      _longitude;       
    double      _latitude;     
  public:   

  // Methods
  private:
  protected:
  public:
    double getLongitude() const { return _longitude; }
    double getLatitude() const { return _latitude; }
  
    Tracker(const double longitude, const double latitude);
    ~Tracker();
};

typedef std::shared_ptr<Tracker>   SpTracker;

//-----------------------------------------------------------------------------

