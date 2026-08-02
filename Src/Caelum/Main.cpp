
// Main.cpp
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// Caelum

#include "Hipparcos.h"
#include "Scope.h"
//----------------------------------------------------------------------------

#define LAT_AUSTIN 30.2672f
#define LONG_AUSTIN -97.7431f


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int main(void)
{
std::string     filepath = "E:\\Stars\\Hipparcos\\main";
Hipparcos       hipparcos(filepath);
Catalog         catalog;
bool            success(false);

  if (hipparcos.loadStarCatalog(catalog) == 0)
  {
  Scope     scope(LONG_AUSTIN, LAT_AUSTIN, 50.0f, 245.0f, glm::vec2(7.0f, 7.0f),0.006);
  double    currentGMST = 210.0;
  StarField visibleStars;

    scope.pointAt(currentGMST,56.75,24.11);

    if (catalog.filter(scope, visibleStars) == 0)
    {
    cv::Mat image;

      scope.projectToImage(visibleStars, image);

      cv::imshow("Visible Stars", image);
      cv::waitKey(0);

      success = true;
    }
  }

  return success ? 0 : 1;
}


