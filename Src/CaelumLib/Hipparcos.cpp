//-----------------------------------------------------------------------------
// Includes
// System
// 
// Project
#include "Hipparcos.h"
//-----------------------------------------------------------------------------
 


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int Hipparcos::getStar(const std::string &line,Star &star) const
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
    star._pmRA = safeToFloat(token,0.0f);
  else
    success = false;

  if (getToken(line,token,163,169) == 0)
    star._pmDec = safeToFloat(token,0.0f);
  else
    success = false;

  if (getToken(line,token,191,197) == 0)
    star._magnitude = safeToFloat(token,99.0f);
  else
    success = false;

  if (getToken(line,token,203,209) == 0)
    star._bvColorIndex = safeToFloat(token,0.0f);
  else
    success = false;

/*
  std::cout << star._catalogID  << " " 
            << star._ra         << " " 
            << star._dec        << " "
            << star._pmRA       << " "  
            << star._pmDec      << " "
            << star._blueMagnitude  << " " 
            << star._bvColorIndex << std::endl;
*/

  return success ? 0 : 1;
}


//-----------------------------------------------------------------------------
// loadStarCatalog
//-----------------------------------------------------------------------------
int Hipparcos::loadStarCatalog(Catalog& catalog) const 
{
std::ifstream file(_filePath);
bool         success(false);

  if (file.is_open()) 
  {
  std::string line;
  size_t parsedCount = 0;
  size_t skippedCount = 0;

    while (std::getline(file, line)) 
    {
      if (!line.empty() && (line[0] != '#')) 
      {
      Star star{};   

        if (getStar(line,star) == 0)
        {
          catalog.add(star);
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
    std::cerr << "Error: Could not open Hipparcos file: " << _filePath << std::endl;

  return success ? 0 : 1;
}



//-----------------------------------------------------------------------------
// Hipparcos
//-----------------------------------------------------------------------------
Hipparcos::Hipparcos(const std::string &filepath) : Parser(filepath)
{
}


//-----------------------------------------------------------------------------
// ~Hipparcos
//-----------------------------------------------------------------------------
Hipparcos::~Hipparcos()
{

}