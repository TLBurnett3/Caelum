//-----------------------------------------------------------------------------
// Includes
// System
// 
// Project
#include "HipparcosLoader.h"
//-----------------------------------------------------------------------------
 

// Converts Greenwich Mean Sidereal Time (in degrees) to Austin's Zenith Ray
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Ray HipparcosLoader::getAustinZenithRay(double gmstDegrees, glm::vec3 origin) const 
{
    constexpr double AUSTIN_LAT = 30.2672;   // Declination overhead
    constexpr double AUSTIN_LON = -97.7431;  // West longitude

    // 1. Compute Local Sidereal Time (RA overhead)
    double lstDegrees = std::fmod(gmstDegrees + AUSTIN_LON, 360.0);
    if (lstDegrees < 0.0) lstDegrees += 360.0;

    // 2. Convert to Radians
    float raRad  = glm::radians(static_cast<float>(lstDegrees));
    float decRad = glm::radians(static_cast<float>(AUSTIN_LAT));

    // 3. Convert to 3D Celestial Unit Vector
    // glm::euclidean expects vec2(latitude/dec, longitude/ra)
    glm::vec3 zenithDir = glm::euclidean(glm::vec2(decRad, raRad));

    return Ray{ origin, glm::normalize(zenithDir) };
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int HipparcosLoader::filterFoV(Star &star,Ray &ray) const
{
glm::vec3 v = celestialToEuclidean(star);


  return 0;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int HipparcosLoader::getStar(const std::string &line,Star &star) const
{
std::string       token;
bool              success(true); 

  if (getToken(line,token,1,7) == 0)
    star._catalogID = static_cast<uint32_t>(safeToDouble(token,0.0));
  else
    success = false;
       
  if (getToken(line,token,58,68) == 0)
    star._ra = safeToDouble(token,0.0);
  else
    success = false;

  if (getToken(line,token,69,79) == 0)
    star._dec = safeToDouble(token,0.0);
  else
    success = false;

  if (getToken(line,token,156,162) == 0)
    star._pmRA = static_cast<float>(safeToDouble(token,0.0));
  else
    success = false;

  if (getToken(line,token,163,169) == 0)
    star._pmDec = static_cast<float>(safeToDouble(token,0.0));
  else
    success = false;

  if (getToken(line,token,191,197) == 0)
    star._magnitude = static_cast<float>(safeToDouble(token,99.0));
  else
    success = false;

  if (getToken(line,token,203,209) == 0)
    star._colorIndex = static_cast<float>(safeToDouble(token,0.0));
  else
    success = false;


  std::cout << star._catalogID  << " " 
            << star._ra         << " " 
            << star._dec        << " "
            << star._pmRA       << " "  
            << star._pmDec      << " "
            << star._magnitude  << " " 
            << star._colorIndex << std::endl;

  return success ? 0 : 1;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int HipparcosLoader::loadCatalog(std::vector<Star>& outStars) const 
{
std::ifstream file(_filepath);
bool         success(false);


  if (file.is_open()) 
  {
  std::string line;
  Ray  ray = getAustinZenithRay(0.0); // Placeholder GMST, replace with actual value if needed
  size_t parsedCount = 0;
  size_t skippedCount = 0;

    outStars.reserve(120000);

    while (std::getline(file, line)) 
    {
      if (!line.empty() && (line[0] != '#')) 
      {
      Star star{};   

        if (getStar(line,star) == 0)
        {
          if (filterFoV(star,ray) == 0)
          {
            outStars.push_back(star);
            parsedCount++;
          }
        }
        else
          skippedCount++;
      }
    }

    std::cout << "Successfully loaded " << parsedCount << " stars ("
              << skippedCount << " entries skipped)." << std::endl;

    success = true; 
  }
  else
    std::cerr << "Error: Could not open Hipparcos file: " << _filepath << std::endl;

  return success ? 0 : 1;
}
