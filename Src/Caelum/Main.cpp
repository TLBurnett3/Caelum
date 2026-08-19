
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
SpScope         spSeeStarS30Pro;
SpScope         spTest;
std::filesystem::path outPath("e:/Caelum");
int             rc = 0;


  rc |= index.loadStarCards(catalog);  
  rc |= hipparcos.loadStarCatalog(catalog);
  rc |= tycho2.loadStarCatalog(catalog);

  if (rc == 0)
  {
  SpTracker spTracker     = std::make_shared<Tracker>(LONG_AUSTIN, LAT_AUSTIN);
  SpOTA     spOTA         = std::make_shared<OTA>(30.0,160.0);
  SpSensor  spSensor      = std::make_shared<Sensor>(spOTA,glm::vec2(11.3f,6.3f),0.006f); // supposed to be 0.0029
  
    spSeeStarS30Pro       = std::make_shared<Scope>("SeeStar S30 Pro",spTracker,spOTA,spSensor);

    spSeeStarS30Pro->setOutPath(outPath);
  }

  if (rc == 0)
  {
  SpTracker spTracker     = std::make_shared<Tracker>(LONG_AUSTIN, LAT_AUSTIN);
  SpOTA     spOTA         = std::make_shared<OTA>(50.0,245.0);
  SpSensor  spSensor      = std::make_shared<Sensor>(spOTA,glm::vec2(7.0f,7.0f),0.006f);// supposed to be 0.002

    spTest                = std::make_shared<Scope>("Test",spTracker, spOTA, spSensor);
     spTest->setOutPath(outPath);
  }

  if (rc == 0)
  {
  double    exposureTime  = 10.0;  // secs
  double    trackTime     = 60 * 60 * 1; // seconds * minutes * hours
  double    currentGMST   = 210.0;
  uint32_t  numExposures  = (uint32_t)(trackTime / exposureTime);
 
   spTest->captureImageSet(catalog,"Alpha Centauri A",currentGMST,exposureTime,numExposures,true);
   spSeeStarS30Pro->captureImageSet(catalog,"Alpha Centauri A",currentGMST,exposureTime,numExposures,true);

    cv::waitKey(0);
  }

  return rc;
}


