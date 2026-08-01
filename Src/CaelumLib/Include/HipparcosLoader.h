#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

// Project
#include "StarDef.h"
//-----------------------------------------------------------------------------
 
//-----------------------------------------------------------------------------
// Classes
class HipparcosLoader 
{
  // Members
  private:
    std::string _filepath;

  // Methods
  private:

    // Helper to safely convert string to double/float, handling blank fields
    static double safeToDouble(const std::string& str, double defaultValue = 0.0) 
    {
      if (str.empty() || std::all_of(str.begin(), str.end(), ::isspace))
      {
        return defaultValue;
      }

      try 
      {
        return std::stod(str);
      } 
      catch (...) 
      {
        return defaultValue;
      }
    }

    static int getToken(const std::string& line, std::string& token, uint32_t startCol, uint32_t endCol) 
    {
    uint32_t sC = startCol - 1;
    uint32_t eC = endCol - 1;
      
      if ((sC < eC) &&(sC < line.size()) && (eC < line.size())) 
      {
        token = line.substr(sC,eC-sC);
        return 0;
      }

      return 1; 
    }

    static float safeToFloat(const std::string& str, float defaultValue = 0.0f) 
    {
      return static_cast<float>(safeToDouble(str, defaultValue));
    }

    int getStar(const std::string& line, Star &star) const;
  
  public:
    explicit HipparcosLoader(std::string filepath) : _filepath(std::move(filepath)) {}

    int loadCatalog(std::vector<Star>& outStars) const;
};
//-----------------------------------------------------------------------------