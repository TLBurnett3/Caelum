
// Parser.h
// Thomas Burnett

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
#include <filesystem>

// 3rdPartyLibs

// Caelum
#include "Catalog.h"

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes

class Parser
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
  protected:
    std::filesystem::path _filePath;
  public:   

  // Methods
  private:
  protected:
    static int getToken(const std::string& line, std::string& token, uint32_t startCol, uint32_t endCol) 
    {
    uint32_t sC = startCol - 1;
    uint32_t eC = endCol;
      
      if ((sC < eC) &&(sC < line.size()) && (eC < line.size())) 
      {
        token = line.substr(sC,eC-sC);
        return 0;
      }

      return 1; 
    }

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

    static float safeToFloat(const std::string& str, float defaultValue = 0.0f) 
    {
      if (str.empty() || std::all_of(str.begin(), str.end(), ::isspace))
      {
        return defaultValue;
      }

      try 
      {
        return std::stof(str);
      } 
      catch (...) 
      {
        return defaultValue;
      }
    }

    static uint32_t safeToUInt(const std::string& str, uint32_t defaultValue = 0) 
    {
      if (str.empty() || std::all_of(str.begin(), str.end(), ::isspace))
      {
        return defaultValue;
      }

      try 
      {
        return std::stoul(str);
      } 
      catch (...) 
      {
        return defaultValue;
      }
    }

    uint32_t getSafeUInt(const std::string &line,
                               const uint32_t startCol,const uint32_t endCol,
                               uint16_t defaultValue = 0) const
    {
    std::string  token;

      if (getToken(line,token,startCol,endCol) == 0)
        return safeToUInt(token,defaultValue);

      return defaultValue;
    }

    double getSafeDouble(const std::string &line,
                         const uint32_t startCol,const uint32_t endCol,
                         double defaultValue = 0.0) const
    {
    std::string  token;

      if (getToken(line,token,startCol,endCol) == 0)
        return safeToDouble(token,defaultValue);

      return defaultValue;
    }

    float getSafeFloat(const std::string &line,
                         const uint32_t startCol,const uint32_t endCol,
                         float defaultValue = 0.0f) const
    {
    std::string  token;

      if (getToken(line,token,startCol,endCol) == 0)
        return safeToFloat(token,defaultValue);

      return defaultValue;
    }

    char getSafeChar(const std::string &line,
                         const uint32_t startCol,const uint32_t endCol,
                         char defaultValue = NULL) const
    {
    std::string  token;

      if (getToken(line,token,startCol,endCol) == 0)
        return token[0];

      return defaultValue;
    }
  public:
    virtual int loadStarCatalog(Catalog& catalog) const = 0;   

    Parser(const std::string &_filepath);
    ~Parser();
};

//-----------------------------------------------------------------------------

