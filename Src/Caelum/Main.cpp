
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
  Scope     scope(LONG_AUSTIN, LAT_AUSTIN, 50.0f, 245.0f, 7.0f);
  double    currentGMST = 210.0;
  StarField visibleStars;

    scope.setZenithRay(currentGMST);

    if (catalog.filter(scope, visibleStars) == 0)
      success = true;
  }

  return success ? 0 : 1;
}


