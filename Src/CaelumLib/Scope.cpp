
// Scope.cpp 
// Thomas Burnett


//---------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// CRos
#include "Scope.h"

//---------------------------------------------------------------------


//---------------------------------------------------------------------
// Scope
//---------------------------------------------------------------------
Scope::Scope(const float ra, const float dec, 
             const float aperature, 
             const float focalLength,
             const float sensorSize) : _posLatLong(glm::vec2(ra, dec)),
                                       _aperature(aperature),
                                       _focalLength(focalLength),
                                       _sensorSize(sensorSize),                                        
                                       _focalRatio(focalLength / aperature)
{
}

//---------------------------------------------------------------------
// ~Scope
//---------------------------------------------------------------------
Scope::~Scope()
{

}

