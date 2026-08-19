
// Catalog.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System


// 3rdPartyLibs

// Caelum
#include "Star.h"
#include "Card.h"
#include "Region.h"
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes

class Catalog
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
    StarIndex     _index;         // Index of star names, positions, magnitudes, etc.
    CelestialSky  _celestialsky;  // Sky partitioned into regions
  protected:
  public:   

  // Methods
  private:
  protected:
  public:
    size_t indexSize()   const { return _index.size(); }
    size_t numRegions(void) const { return _celestialsky.size(); }

    const Card* getCardByName(const char *pN)
    { 
    std::string name(pN);
    const Card *pCard = 0;

      if (_index.find(name) != _index.end())
        pCard = &_index[name];

      return pCard; 
    }
    
    const Region *region(const uint32_t raDeg,const uint32_t decDeg)
    { 
    uint32_t i = (decDeg * 360) + raDeg;

      return &_celestialsky[i];
    }

    const Region *region(const size_t i)
    { 
      return &_celestialsky[i];
    }

    void add(const Card& card) 
    { _index[card.commonName()] = card; }

    void add(const Star& star) 
    { 
    uint32_t i = star.region();

      _celestialsky[i].add(star);
    }
  
    Catalog(void);
    ~Catalog();
};

//-----------------------------------------------------------------------------

