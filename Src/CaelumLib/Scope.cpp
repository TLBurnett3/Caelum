
// Scope.cpp 
// Thomas Burnett


//---------------------------------------------------------------------
// Includes
// System
#include <iostream>

// 3rdPartyLibs
 #include <glm/gtc/constants.hpp>

// CRos
#include "Scope.h"

//---------------------------------------------------------------------

//---------------------------------------------------------------------
// Constants and Defines

constexpr double EARTH_SIDEREAL_DEG_PER_SEC = 360.0 / 86164.0905; // ~0.0041780746 deg/s
//---------------------------------------------------------------------



//-----------------------------------------------------------------------------
// track
//-----------------------------------------------------------------------------
int Scope::track(Catalog &catalog,
                  const char *pStr, const double ra, const double dec,
                  const double gmstDegrees,const double tDelta,
                  const uint32_t nFrames,ImageLst &imageLst)
{
double    curLat    = _spTracker->getLatitude();
double    curLong   = _spTracker->getLongitude();
double    curLST    = gmstDegrees + curLong;
glm::vec3 vWorldUp  = glm::vec3(0.0f, 0.0f, 1.0f); // Celestial North Pole
glm::vec3 vDir      = geographicToEuclidean(ra,dec);

  for (uint32_t i = 0; i < nFrames; ++i)
  {
  cv::Mat image  = cv::Mat::zeros(_spSensor->imageHeight(),_spSensor->imageWidth(), CV_8UC3);

    curLST += tDelta * EARTH_SIDEREAL_DEG_PER_SEC;

    double LST = normalize360(curLST);

    glm::vec3 vPos   = geographicToEuclidean(LST,dec);
    glm::vec3 vRight = glm::normalize(glm::cross(vDir, vPos));  // vPos is for altaz mount, vWorldUp is for equatorial mount
    glm::vec3 vUp    = glm::cross(vRight, vDir); 

    for (size_t j = 0; j < catalog.size(); ++j)
    {
    const Star *pStar = catalog.getStarByID(j);
    glm::vec3  vStar  = celestialToEuclidean(*pStar);         

      double zCam = glm::dot(vStar,vDir);

      if (zCam > 0)
      {       
      // Project star onto Camera Right (x) and Up (y) axes
      double    xCam       = glm::dot(vStar, vRight);   // Horizontal displacement
      double    yCam       = glm::dot(vStar, vUp);      // Vertical displacement
      // Project star onto Camera Right (x) and Up (y) axes
      double    xTangent   = xCam / zCam;
      double    yTangent   = yCam / zCam;

        _spSensor->render(*pStar,xTangent,yTangent,image);
      }
    }

    std::cout << i << ":" << LST << std::endl;

    cv::imshow(pStr,image);
    cv::waitKey(100);

    imageLst.push_back(image);
  }

  return 0;
}

//---------------------------------------------------------------------
// Scope
//---------------------------------------------------------------------
Scope::Scope(const float longitude, const float latitude, 
             const float aperature, 
             const float focalLength,
             const glm::vec2 sensorSize,
             const float  pixelSize,
             const float toleranceDeg) :_longitude(longitude),
                                        _latitude(latitude),
                                        _lst(longitude),
                                       _direction(0.0f,0.0f,1.0f),
                                       _aperature(aperature),
                                       _focalLength(focalLength),
                                       _sensorSize(sensorSize),             
                                       _pixelSize(pixelSize),
                                       _focalRatio(focalLength / aperature),
                                       _fovYDeg(0.0f),
                                       _cosThreshold(0.0f)   
{
  _imageSize = glm::ivec2(static_cast<int>(_sensorSize.x / _pixelSize),
                          static_cast<int>(_sensorSize.y / _pixelSize));
  _fovYDeg       = 2.0f * glm::degrees(atan((_sensorSize.y / 2.0f) / _focalLength));
  _cosThreshold  = std::cos(glm::radians(toleranceDeg));
}




//---------------------------------------------------------------------
// Scope
//---------------------------------------------------------------------
Scope::Scope(SpTracker& spTracker, SpOTA& spOTA, SpSensor& spSensor) : _spTracker(spTracker),
                                                                       _spOTA(spOTA),
                                                                       _spSensor(spSensor)
{
}

//---------------------------------------------------------------------
// ~Scope
//---------------------------------------------------------------------
Scope::~Scope()
{

}

