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
  float _magnitude;      // Apparent visual magnitude (V or G band)
  float _colorIndex;     // BP - RP or B - V (useful for rendering temperature/color)
    
  // Identifiers
  uint32_t _catalogID;   // Identifier (e.g., Tycho-2 ID, UCAC4 ID, or local index)
  uint16_t _flags;       // Bitfield for metadata (e.g., variable star, binary, quality flag)
  uint8_t _reserved[2];  // Padding to maintain exact 32-byte alignment
};

typedef std::vector<Star> StarCatalog;
typedef std::vector<Star> StarField;

struct Ray 
{
  glm::vec3 _origin;    // Starting point (e.g., Earth center or surface)
  glm::vec3 _direction; // Unit vector pointing straight up into space
};
//-----------------------------------------------------------------------------
