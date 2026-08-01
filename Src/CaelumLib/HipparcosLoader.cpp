//-----------------------------------------------------------------------------
// Includes
// System
// 
// Project
#include "HipparcosLoader.h"
//-----------------------------------------------------------------------------
 

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
  size_t parsedCount = 0;
  size_t skippedCount = 0;

    outStars.reserve(120000);

    while (std::getline(file, line)) 
    {
      if (!line.empty() && (line[0] != '#')) 
      {
      Star star{};   

        if (getStar(line, star) == 0)
        {
          outStars.push_back(star);
          parsedCount++;
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
