
// Scope.cpp 
// Thomas Burnett


//---------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// CRos
#include "Scope.h"

//---------------------------------------------------------------------



// Converts Greenwich Mean Sidereal Time (in degrees) to Austin's Zenith Ray
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Scope::setZenithRay(double gmstDegrees)
{
  // 1. Compute Local Sidereal Time (RA overhead)
  double lstDegrees = std::fmod(gmstDegrees + _position.y, 360.0);
  if (lstDegrees < 0.0) 
    lstDegrees += 360.0;

  // 2. Convert to Radians
  float raRad  = glm::radians(static_cast<float>(lstDegrees));
  float decRad = glm::radians(static_cast<float>(_position.x));

  // 3. Convert to 3D Celestial Unit Vector
  // glm::euclidean expects vec2(latitude/dec, longitude/ra)
  _direction = glm::euclidean(glm::vec2(decRad, raRad));
}


//-----------------------------------------------------------------------------
// isVisible
//-----------------------------------------------------------------------------
int Scope::isVisible(const Star& star) const
{
glm::vec3 v = celestialToEuclidean(star);

  return 0;
}



//---------------------------------------------------------------------
// Scope
//---------------------------------------------------------------------
Scope::Scope(const float ra, const float dec, 
             const float aperature, 
             const float focalLength,
             const float sensorSize) : _position(glm::vec2(ra, dec)),
                                       _direction(0.0f,0.0f,1.0f),
                                       _aperature(aperature),
                                       _focalLength(focalLength),
                                       _sensorSize(sensorSize),                                        
                                       _focalRatio(focalLength / aperature),
                                       _fieldOfView(0.0f)    
{
  _fieldOfView = 2.0f * glm::degrees(atan((_sensorSize / 2.0f) / _focalLength));
}



//---------------------------------------------------------------------
// ~Scope
//---------------------------------------------------------------------
Scope::~Scope()
{

}

