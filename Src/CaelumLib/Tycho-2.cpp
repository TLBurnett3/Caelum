
// Tycho2.cpp 
// Thomas Burnett


//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// CRos
#include "Tycho-2.h"

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int Tycho2::getStar(const std::string &line,Star &star) const
{
std::string       token;
int               rc = 0;

  // ID
  {
  uint16_t tyc1 = static_cast<uint16_t>(getSafeUInt(line,1,4));
  uint16_t tyc2 = static_cast<uint16_t>(getSafeUInt(line,6,10));
  uint16_t tyc3 = static_cast<uint16_t>(getSafeUInt(line,12,12));

 //   if ((tyc1 == 4628) && (tyc2 == 237))
 //     std::cout << line << std::endl;

    star._catalogID = packTychoID(tyc1, tyc2, tyc3);
  }
       
  // Position
  if (getSafeChar(line,14,14) != 'X')
  {
    star._ra    = getSafeDouble(line,16,27);
    star._dec   = getSafeDouble(line,29,40);
    star._pmRA  = getSafeFloat(line,42,48);
    star._pmDec = getSafeFloat(line, 50, 56);
  }
  else
  {
    star._ra    = getSafeDouble(line,153,164);
    star._dec   = getSafeDouble(line,166,177);
    star._pmRA  = 0.0f;
    star._pmDec = 0.0f;
  }

  // Magnitude & Color
  {
  float BT = 0.0f;
  float VT = 0.0f;

    BT = getSafeFloat(line, 111,116);
    VT = getSafeFloat(line, 124, 129);

    float delta = BT - VT;  

    star._bvColorIndex = 0.850f * delta;
    star._magnitude    = VT - 0.090f * delta;
  }

/*
  std::cout << star._catalogID  << " " 
            << star._ra         << " " 
            << star._dec        << " "
            << star._pmRA       << " "  
            << star._pmDec      << " "
            << star._magnitude  << " " 
            << star._bvColorIndex << std::endl;
*/

  return rc;
}


//-----------------------------------------------------------------------------
// loadTypcho2
//-----------------------------------------------------------------------------
int Tycho2::loadTypcho2(Catalog& catalog,const std::filesystem::path& fileName) const
{
std::filesystem::path filePath = _filePath;

filePath /= "Data";
filePath /= fileName;

std::ifstream file(filePath);
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
    std::cerr << "Error: Could not open Hipparcos file: " << _filepath << std::endl;

  return success ? 0 : 1;
}


//-----------------------------------------------------------------------------
// loadStarCatalog
//-----------------------------------------------------------------------------
int Tycho2::loadStarCatalog(Catalog& catalog) const 
{
int rc = 0;

  rc |= loadTypcho2(catalog,std::filesystem::path("suppl_1.dat"));
  rc |= loadTypcho2(catalog,std::filesystem::path("suppl_2.dat"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.00"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.01"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.02"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.03"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.04"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.05"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.06"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.07"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.08"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.09"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.10"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.11"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.12"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.13"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.14"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.15"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.16"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.17"));  
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.18"));
  rc |= loadTypcho2(catalog,std::filesystem::path("tyc2.dat.19"));

  return rc;
}


//-----------------------------------------------------------------------------
// Tycho2
//-----------------------------------------------------------------------------
Tycho2::Tycho2(const std::string &filepath) : Parser(filepath)
{
}


//-----------------------------------------------------------------------------
// ~Tycho2
//-----------------------------------------------------------------------------
Tycho2::~Tycho2()
{

}

