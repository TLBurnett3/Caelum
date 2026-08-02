
// Main.cpp
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// Caelum

#include "HipparcosLoader.h"
#include "Scope.h"
//----------------------------------------------------------------------------

#define LAT_AUSTIN 30.2672f
#define LONG_AUSTIN -97.7431f


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int main(void)
{
std::string     filepath = "E:\\Stars\\Hipparcos\\main";
HipparcosLoader hipparcosLoader(filepath);
StarCatalog     starCatalog;
Scope           scope(LONG_AUSTIN, LAT_AUSTIN, 50.0f, 245.0f, 7.0f);
bool            success(false);

  if (hipparcosLoader.loadCatalog(starCatalog) == 0)
  {
    success = true;
  }

  return success ? 0 : 1;
}


