
// Catalog.cpp 
// Thomas Burnett


//-----------------------------------------------------------------------------
// Includes
// System
#include <cmath>

// 3rdPartyLibs

// CRos
#include "Catalog.h"

//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
// Catalog
//-----------------------------------------------------------------------------
Catalog::Catalog(void) : _index(),
                          _catalog()
{
double   stp  = 1.0;

  _sky.resize(360 * 180);

  for (double ra = 0;ra < 360.0;ra += stp)
  {
    for (double dec = 0.0;dec < 180.0;dec += stp)
    {
    uint32_t i = static_cast<uint32_t>(floor(ra * dec));

      _sky[i] = Region(i,ra,ra + stp,dec - 90.0,dec - 90.0 + stp);
    }
  }

  _index.reserve(1000);
  _catalog.reserve(1000000); 
}


//-----------------------------------------------------------------------------
// ~Catalog
//-----------------------------------------------------------------------------
Catalog::~Catalog()
{

}

