
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
// ~Scope
//---------------------------------------------------------------------
Scope::~Scope()
{

}

