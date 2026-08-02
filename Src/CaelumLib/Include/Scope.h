
// Scope.h
// Thomas Burnett

#pragma once

//---------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs
#include <glm/glm.hpp>
#include <glm/gtx/polar_coordinates.hpp>

// Caelum

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
    glm::vec2 _posLatLong;  // surface origin
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
  protected:
  public:
  
    Scope(const float ra, const float dec, const float aperature, const float focalLength, const float sensorSize);
    ~Scope();
};

//---------------------------------------------------------------------

