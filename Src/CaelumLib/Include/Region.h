
// Region.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// Caelum
#include "Star.h"
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes

class Region
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
  protected:
    uint32_t  _id;
    double    _minRaDeg;
    double    _maxRaDeg;
    double    _minDecDeg;
    double    _maxDecDeg;

    StarField _starField;
  public:   


  // Methods
  private:
  protected:
  public:

    size_t fieldSize() const { return _starField.size(); }

    const Star* getStar(size_t i) const
    { return &_starField[i]; }

    void add(const Star& star) 
    { 
      _starField.push_back(star);
    }
  
    Region(const uint32_t id,
           const double minRaDeg,const double maxRaDeg,
           const double minDecDeg,const double maxDecDeg) : _id(id),
                                                            _minRaDeg(minRaDeg),
                                                            _maxRaDeg(maxRaDeg),
                                                            _minDecDeg(minDecDeg),
                                                            _maxDecDeg(maxDecDeg)
    {
/*
      std::cout << id         << " "
                << _minRaDeg  << " "  
                << _maxRaDeg  << " "  
                << _minDecDeg << " "  
                << _maxDecDeg << " " 
                << _starField.size() << std::endl;
*/
    }

    Region(void)
    {}

    ~Region()
    {
    }
};

typedef std::vector<Region> CelestialSky;
//-----------------------------------------------------------------------------

