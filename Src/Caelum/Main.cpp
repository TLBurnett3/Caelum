
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

#define RA_NORTHPOLE 0.0
#define DEC_NORTHPOLE 90.0

#define RA_POLARIS 37.954560
#define DEC_POLARIS 89.264108

#define RA_MILKYWAY 308.0
#define DEC_MILKYWAY 40.0 

#define RA_VEGA 279.2347
#define DEC_VEGA 38.7837


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void capture(Catalog& catalog, SpScope spScope, const char *pTargetName, 
             double ra, double dec, double currentGMST, int exposureTime, int numFrames)
{
 ImageLst imageStk;

  spScope->track(catalog, pTargetName, ra, dec, currentGMST, exposureTime, numFrames, imageStk);
}

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
  SpTracker spTracker   = std::make_shared<Tracker>(LONG_AUSTIN, LAT_AUSTIN);
  SpOTA     spOTA       = std::make_shared<OTA>( 50.0f, 245.0f);
  SpSensor  spSensor    = std::make_shared<Sensor>(glm::vec2(7.0f, 7.0f),0.006f, 245.0f);
  SpScope   spScope     = std::make_shared<Scope>(spTracker, spOTA, spSensor);
  double    currentGMST = 210.0;

    capture(catalog,spScope,"Pleades",RA_PLEADES,DEC_PLEADES,currentGMST,60,120);
    capture(catalog,spScope,"Polaris",RA_POLARIS,DEC_POLARIS,currentGMST,60,120);
    capture(catalog,spScope,"Milky Way",RA_MILKYWAY,DEC_MILKYWAY,currentGMST,60,120);
    capture(catalog,spScope,"VEGA",RA_VEGA,DEC_VEGA,currentGMST,60,120);
 
    cv::waitKey(0);
  }

  return rc;
}


