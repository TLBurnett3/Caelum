
// Scope.h
// Thomas Burnett

#pragma once

//---------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// Caelum
#include "Star.h"
//---------------------------------------------------------------------


//---------------------------------------------------------------------
// Classes

class Scope
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
    glm::vec2 _position;    // surface origin (polar)
    glm::vec3 _direction;   // Unit vector pointing straight up into space
    float _aperature;       // Size of the telescope's opening (in mm)
    float _focalLength;     // Distance from the lens to the focal point (in mm) 
    float _sensorSize;      // Size of the camera sensor (in mm)
    float _focalRatio;      // Ratio of focal length to aperature (focalLength / aperature)
    float _fieldOfView;     // Angular extent of the observable sky (in degrees)

  protected:
  public:   

  // Methods
  private:
    glm::vec3 celestialToEuclidean(const Star &star) const
    {
      // 1. Convert degrees to radians
      float raRad  = glm::radians(static_cast<float>(star._ra));
      float decRad = glm::radians(static_cast<float>(star._dec));

      // 2. Form GLM polar vector: vec2(latitude/dec, longitude/ra)
      glm::vec2 polar(decRad, raRad);

      // 3. Convert to 3D Cartesian unit vector (x, y, z)
      glm::vec3 dir = glm::euclidean(polar);

      return dir;
    }  
  protected:
  public:
    void setZenithRay(double gmstDegrees);

    int isVisible(const Star& star) const;
  
    Scope(const float ra, const float dec, const float aperature, const float focalLength, const float sensorSize);
    ~Scope();
};

//---------------------------------------------------------------------

