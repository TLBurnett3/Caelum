
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
//-----------------------------------------------------------------------------
void Scope::projectToImage(const StarField &starField, cv::Mat &image)
{
    float halfHeight  = static_cast<float>(_imageSize.y) * 0.5f;
    float halfWidth   = static_cast<float>(_imageSize.x) * 0.5f;
    float halfFovYRad = glm::radians(_fovYDeg * 0.5f);

    // Focal length in pixels
    float focalScale  = halfHeight / std::tan(halfFovYRad);

    // --- Build Camera Frame from _direction ---
    glm::vec3 forward = glm::normalize(_direction);

    // North Celestial Pole reference vector
    glm::vec3 worldUp(0.0f, 0.0f, 1.0f);

    // Handle edge case if scope is pointing directly at Celestial North/South Pole
    if (std::abs(glm::dot(forward, worldUp)) > 0.999f) {
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // Right vector (points East / +X camera)
    glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
    
    // Camera Up vector (+Y camera)
    glm::vec3 up = glm::normalize(glm::cross(right, forward));

    // Optional: Sensor Roll (Rotation around Forward axis)
    float rollRad = glm::radians(0.0f);
    if (rollRad != 0.0f) {
        glm::vec3 rOld = right;
        glm::vec3 uOld = up;
        right = rOld * std::cos(rollRad) - uOld * std::sin(rollRad);
        up    = rOld * std::sin(rollRad) + uOld * std::cos(rollRad);
    }

    image = cv::Mat::zeros(_imageSize.y, _imageSize.x, CV_8UC3);

    for (const auto& star : starField) 
    {
        // 1. Get Star 3D Unit Vector (RA/Dec -> XYZ)
        double ra  = glm::radians(star._ra);
        double dec = glm::radians(star._dec);
        double cosDec = std::cos(dec);

        glm::vec3 starVec(
            static_cast<float>(cosDec * std::cos(ra)),
            static_cast<float>(cosDec * std::sin(ra)),
            static_cast<float>(std::sin(dec))
        );

        // 2. Project onto Camera Axes using Dot Products
        float zCam = glm::dot(starVec, forward); // Distance along sight line (denom)

        // Reject stars behind or perpendicular to the camera plane
        if (zCam <= 0.0f) continue;

        float xCam = glm::dot(starVec, right);   // Horizontal displacement
        float yCam = glm::dot(starVec, up);      // Vertical displacement

        // 3. Perspective / Gnomonic Projection to Screen Pixels
        // Note: For East = Left (astronomical standard view), use -xCam
        float pixelX = halfWidth  - (xCam / zCam) * focalScale; 
        float pixelY = halfHeight - (yCam / zCam) * focalScale; // - for screen Y inversion

        // 4. Screen Bounds Check
        bool visible = (pixelX >= 0.0f && pixelX < static_cast<float>(_imageSize.x) &&
                        pixelY >= 0.0f && pixelY < static_cast<float>(_imageSize.y));

        if (visible) {
            std::cout << "Star ID: " << star._catalogID 
                      << ", Pixel Coordinates: (" << pixelX << ", " << pixelY << ")\n";

            cv::circle(image, cv::Point(static_cast<int>(pixelX), static_cast<int>(pixelY)), 
                       1, cv::Scalar(255, 255, 255), -1);
        }
    }
}
// Converts Greenwich Mean Sidereal Time (in degrees) to Austin's Zenith Ray
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void Scope::pointAt(double gmstDegrees, double raT, double decT)
{
  // 1. Compute Local Sidereal Time (RA overhead)
  _lst = std::fmod(gmstDegrees + _longitude, 360.0);
  if (_lst < 0.0) 
    _lst += 360.0;

// 1. Calculate Hour Angle
  double haDeg = _lst - raT;

    // Convert all inputs to radians
  double latRad = glm::radians(_latitude);
  double decRad = glm::radians(decT);
  double haRad  = glm::radians(haDeg);

  // 2. Compute Altitude
  double sinAlt = std::sin(latRad) * std::sin(decRad) + 
                  std::cos(latRad) * std::cos(decRad) * std::cos(haRad);
  sinAlt = glm::clamp(sinAlt, -1.0, 1.0);
  double altRad = std::asin(sinAlt);

  // 3. Compute Azimuth
  double cosAlt = std::cos(altRad);
  double cosAz = (std::sin(decRad) - std::sin(latRad) * sinAlt) / 
                  (std::cos(latRad) * cosAlt);
  cosAz = glm::clamp(cosAz, -1.0, 1.0);
    
  double azRad = std::acos(cosAz);
  if (std::sin(haRad) > 0.0) {
      azRad = glm::two_pi<double>() - azRad; // Target is West of meridian
  }

  double altDeg = glm::degrees(altRad);
  double azDeg  = glm::degrees(azRad);
  bool isVisible = (altDeg > 0.0); // True if above horizon

 // 5. Celestial Direction Vector (for star field projection and isVisible checks)
  double raTRad = glm::radians(raT);
  double cosDecT = std::cos(decRad);

  _direction = glm::vec3(
        static_cast<float>(cosDecT * std::cos(raTRad)), // X
        static_cast<float>(cosDecT * std::sin(raTRad)), // Y
        static_cast<float>(std::sin(decRad)));          // Z

  std::cout << _aperature << "mm Aperature, "
    << _focalLength << "mm Focal Length, "
    << _sensorSize.x << "x" << _sensorSize.y << "mm Sensor Size, "
    << _focalRatio << " Focal Ratio, "
    << _fovYDeg << " deg Vertical Field of View"
    << std::endl; 

  std::cout << "Scope Pointing Direction: ("
    << _direction.x << ", "
    << _direction.y << ", "
    << _direction.z << ")"
    << std::endl;
}


//-----------------------------------------------------------------------------
// isVisible
//-----------------------------------------------------------------------------
bool Scope::isVisible(const Star& star) const
{
glm::vec3 position      = celestialToEuclidean(star);
float     dotProduct    = glm::dot(_direction,position);

  if (dotProduct >= _cosThreshold)
  {
  float angleRad = std::acos(glm::clamp(dotProduct,-1.0f, 1.0f));
  float angleDeg = glm::degrees(angleRad);

  std::cout << "Star ID: " << star._catalogID
    << " RA: " << star._ra
    << " Dec: " << star._dec
    << " Angle: " << angleDeg
    << std::endl;

    return true;
  }

  return false;
}

// Calculate horizontal Alt/Az coordinates for a celestial target

//-----------------------------------------------------------------------------
// calculateAltAz
//-----------------------------------------------------------------------------
AltAz  Scope::calculateAltAz(double raDeg, double decDeg, double lstDeg, double latDeg) {
    double haDeg  = lstDeg - raDeg;
    
    double latRad = glm::radians(latDeg);
    double decRad = glm::radians(decDeg);
    double haRad  = glm::radians(haDeg);

    // Altitude calculation
    double sinAlt = std::sin(latRad) * std::sin(decRad) + 
                    std::cos(latRad) * std::cos(decRad) * std::cos(haRad);
    sinAlt = glm::clamp(sinAlt, -1.0, 1.0);
    double altRad = std::asin(sinAlt);

    // Azimuth calculation
    double cosAlt = std::cos(altRad);
    double cosAz = (std::sin(decRad) - std::sin(latRad) * sinAlt) / 
                   (std::cos(latRad) * cosAlt);
    cosAz = glm::clamp(cosAz, -1.0, 1.0);

    double azRad = std::acos(cosAz);
    if (std::sin(haRad) > 0.0) { // West of local meridian
        azRad = glm::two_pi<double>() - azRad;
    }

    AltAz result;
    result.altDeg = glm::degrees(altRad);
    result.azDeg  = glm::degrees(azRad);
    result.isVisible = (result.altDeg > 0.0);
    return result;
}

//-----------------------------------------------------------------------------
// track
//-----------------------------------------------------------------------------
int Scope::track(Catalog &catalog,
                  const char *pStr, const double ra, const double dec,
                  const double gmstDegrees,const double tDelta,
                  const uint32_t nFrames,ImageLst &imageLst)
{
double    curLST  = gmstDegrees + ra;

  for (uint32_t i = 0; i < nFrames; ++i)
  {
  cv::Mat image  = cv::Mat::zeros(_spSensor->imageHeight(),_spSensor->imageWidth(), CV_8UC3);

    curLST += tDelta * EARTH_SIDEREAL_DEG_PER_SEC;

    double LST = normalize360(curLST);

    glm::vec3 vWorldUp(0.0f, 0.0f, 1.0f); // Celestial North Pole
    glm::vec3 vPos       = celestialToEuclidean(LST,dec);
    glm::vec3 vRight     = glm::normalize(glm::cross(vPos, vWorldUp));
    glm::vec3 vUp        = glm::normalize(glm::cross(vRight, vPos)); 

    for (size_t j = 0; j < catalog.size(); ++j)
    {
    const Star *pStar = catalog.getStarByID(j);
    glm::vec3  vDir   = celestialToEuclidean(*pStar);         

      vDir = glm::normalize(vDir);

      double zCam = glm::dot(vDir,vPos);

      if (zCam > 0)
      {       
      // Project star onto Camera Right (x) and Up (y) axes
      double    xCam       = glm::dot(vDir, vRight);   // Horizontal displacement
      double    yCam       = glm::dot(vDir, vUp);      // Vertical displacement
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

