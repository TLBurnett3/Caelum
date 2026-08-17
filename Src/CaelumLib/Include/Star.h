#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

// 3rdPartyLibs
#include <glm/glm.hpp>

// Caelum

//-----------------------------------------------------------------------------

// GLM's glm::euclidean expects a glm::vec2 formatted as glm::vec2(latitude, longitude) 
// in radians:
// polar.x (Latitude / Declination $\delta$): Angle up/down from the equator.
// polar.y (Longitude / Right Ascension $\alpha$): Angle wrapped horizontally 
// around the equator.

//-----------------------------------------------------------------------------
// Packed to 32 bytes for cache alignment (2 stars per 64-byte cache line)
class alignas(32) Star 
{
  // Defines
  private:
  protected:
    enum 
    {
      ID_HIPPARCOS = 0x01,
      ID_TYPCO2    = 0x02,
      ID_MASK      = 0x03
    };
  public:

  // Members
  private:
  protected:
    // Spatial & Astrometric Data    
    double _ra;            // Right Ascension in degrees [0, 360)
    double _dec;           // Declination in degrees [-90, 90]
    
    // Proper Motion (mas/yr - milliarcseconds per year)
    float _pmRA;           // Proper motion in RA (cos(dec) applied)
    float _pmDec;          // Proper motion in Dec  

    // Photometry & Physical Properties
    float _vMag;       // visual magnitude (brightness)
    float _bv;    // color index, useful for rendering temperature/color
    
    // Identifiers
    uint32_t _catalogID;   // Identifier (e.g., Tycho2 ID, UCAC4 ID, or local index)
    uint16_t _flags;       // Bitfield for metadata (e.g., variable star, binary, quality flag)
    uint8_t _reserved[2];  // Padding to maintain exact 32-byte alignment
  public:   


  // Methods
  private:
  protected:
  public:
    const uint32_t catalogID(void) const
    { return _catalogID; }

    void setHipparcosID(const uint32_t id)
    {
      _catalogID = id;
      _flags |= ID_HIPPARCOS;
    }

    void setTychoID(const uint16_t tyc1,const uint16_t tyc2,const uint16_t tyc3)
    {
      _catalogID = (static_cast<uint32_t>(tyc1) << 17) |
                   (static_cast<uint32_t>(tyc2) << 3)  |
                   (static_cast<uint32_t>(tyc3) & 0x07);
      _flags |= ID_TYPCO2;
    }


    const double raDeg(void) const
    { return _ra; }

    void setRaDeg(const double raDeg)
    { _ra = raDeg; }

    const double decDeg(void) const
    { return _dec; }

    void setDecDeg(const double decDeg)
    { _dec = decDeg; }


    const double raProperMotion(void) const
    { return _pmRA; }

    void setRaProperMotion(const double raDeg)
    { _pmRA = raDeg; }

    const double decProperMotion(void) const
    { return _pmDec; }

    void setDecProperMotion(const double decDeg)
    { _pmDec = decDeg; }



    const float visualMagnitude(void) const
    { return _vMag; }

    void setVisualMagnitude(const float vMag)
    { _vMag = vMag; }

    const float bvColorIndex(void) const
    { return _bv; }

    void setBVColorIndex(const float bv)
    { _bv = bv; }

    const uint32_t region(void) const
    {
    uint32_t x = std::floor(_ra);
    uint32_t y = std::floor(_dec + 90.0);
    uint32_t r = (y * 360) + x;
 
      return r;
    }

    void print(void) 
    {
      std::cout << _catalogID  << " " 
                << _ra         << " " 
                << _dec        << " "
                << _pmRA       << " "  
                << _pmDec      << " "
                << _vMag       << " " 
                << _bv         << std::endl;
    }

    Star(void)
    {}

    ~Star()
    {}
};

typedef std::vector<Star> StarField;
//-----------------------------------------------------------------------------
