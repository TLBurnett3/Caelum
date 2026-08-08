#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <cstdint>
#include <string>
#include <vector>

// 3rdPartyLibs
#include <glm/glm.hpp>
#include <glm/gtx/polar_coordinates.hpp>

// Caelum

//-----------------------------------------------------------------------------

// GLM's glm::euclidean expects a glm::vec2 formatted as glm::vec2(latitude, longitude) 
// in radians:
// polar.x (Latitude / Declination $\delta$): Angle up/down from the equator.
// polar.y (Longitude / Right Ascension $\alpha$): Angle wrapped horizontally 
// around the equator.

//-----------------------------------------------------------------------------
// Packed to 32 bytes for cache alignment (2 stars per 64-byte cache line)
struct alignas(32) Star 
{
  // Spatial & Astrometric Data
  double _ra;            // Right Ascension in degrees [0, 360)
  double _dec;           // Declination in degrees [-90, 90]
    
  // Proper Motion (mas/yr - milliarcseconds per year)
  float _pmRA;           // Proper motion in RA (cos(dec) applied)
  float _pmDec;          // Proper motion in Dec
    
  // Photometry & Physical Properties
  float _magnitude;       // visual magnitude (brightness)
  float _bvColorIndex;    // color index, useful for rendering temperature/color
    
  // Identifiers
  uint32_t _catalogID;   // Identifier (e.g., Tycho2 ID, UCAC4 ID, or local index)
  uint16_t _flags;       // Bitfield for metadata (e.g., variable star, binary, quality flag)
  uint8_t _reserved[2];  // Padding to maintain exact 32-byte alignment
};

typedef std::vector<Star> StarField;
//-----------------------------------------------------------------------------
