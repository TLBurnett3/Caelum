
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
    StarIndex _index;       // Index of star names, positions, magnitudes, etc.
    StarField _catalog;     // Vector of Star objects representing the star catalog
  protected:
  public:   

  // Methods
  private:
  protected:
  public:
    size_t indexSize()   const { return _index.size(); }
    size_t catalogSize() const { return _catalog.size(); }

    const Star* getStarByID(size_t catalogID) const
    { return &_catalog[catalogID]; }

    void add(const Card& card) { _index[card.commonName()] = card; }
    void add(const Star& star) { _catalog.push_back(star); }
  
    Catalog(void);
    ~Catalog();
};

//-----------------------------------------------------------------------------

