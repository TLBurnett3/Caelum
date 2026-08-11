
// Main.cpp
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// Caelum

#include "Hipparcos.h"
#include "Tycho-2.h"
#include "Index.h"
#include "Scope.h"
//----------------------------------------------------------------------------

#define LAT_AUSTIN 30.2672
#define LONG_AUSTIN -97.7431

#define RA_PLEADES 56.75
#define DEC_PLEADES 56.75

#define RA_MILKYWAY 308.0
#define DEC_MILKYWAY 40.0 


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int main(void)
{
std::string     hipparocsfilepath = "E:\\Stars\\Hipparcos\\main";
std::string     tycho2FilePath    = "E:\\Stars\\Tycho-2";
std::string     indexFilePath     = "E:\\Stars\\Stars.csv";
Hipparcos       hipparcos(hipparocsfilepath);
Tycho2          tycho2(tycho2FilePath);
Index           index(indexFilePath);
Catalog         catalog;
int             rc = 0;

//  rc |= hipparcos.loadStarCatalog(catalog);
  rc |= index.loadStarCards(catalog);
  rc |= tycho2.loadStarCatalog(catalog);

  if (rc == 0)
  {
  double    exposureTime  = 10.0;
  double     aperture     = 50.0;
  double     focalLength  = 245.0;
//  double     aperture    = 200.0;
//  double     focalLength = 800.0;
  SpTracker spTracker     = std::make_shared<Tracker>(LONG_AUSTIN, LAT_AUSTIN);
  SpOTA     spOTA         = std::make_shared<OTA>( aperture, focalLength);
  SpSensor  spSensor      = std::make_shared<Sensor>(spOTA,glm::vec2(7.0f, 7.0f),0.006f);
  SpScope   spScope       = std::make_shared<Scope>(spTracker, spOTA, spSensor);
  double    currentGMST   = 210.0;
  uint32_t  numExposures  = 120;

 //   spScope->capture(catalog,"Pleades",RA_PLEADES,DEC_PLEADES,currentGMST,exposureTime,numExposures);
 //   spScope->capture(catalog,"Milky Way",RA_MILKYWAY,DEC_MILKYWAY,currentGMST,exposureTime,numExposures);
 
    spScope->capture(catalog,"Polaris",currentGMST,exposureTime,numExposures);
 
    cv::waitKey(0);
  }

  return rc;
}


