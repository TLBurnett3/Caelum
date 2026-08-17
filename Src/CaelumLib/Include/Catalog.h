
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
    CelestialSky  _sky;         // Sky partitioned into regions
    StarIndex     _index;       // Index of star names, positions, magnitudes, etc.
    StarField     _catalog;     // Vector of Star objects representing the star catalog
  protected:
  public:   

  // Methods
  private:
  protected:
  public:
    size_t indexSize()   const { return _index.size(); }
    size_t catalogSize() const { return _catalog.size(); }

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

      return &_sky[i];
    }

    const Star* getStarByID(size_t catalogID) const
    { return &_catalog[catalogID]; }

    void add(const Card& card) 
    { _index[card.commonName()] = card; }

    void add(const Star& star) 
    { 
    uint32_t i = star.region();

      _sky[i].add(star);
      _catalog.push_back(star);
    }
  
    Catalog(void);
    ~Catalog();
};

//-----------------------------------------------------------------------------

