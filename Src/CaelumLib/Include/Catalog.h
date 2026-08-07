
// Catalog.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// Caelum
//#include "Scope.h"
#include "Star.h"
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
    StarField _catalog;  // Vector of Star objects representing the star catalog
  protected:
  public:   

  // Methods
  private:
  protected:
  public:
    size_t size() const { return _catalog.size(); }

    const Star* getStarByID(size_t catalogID) const
    { return &_catalog[catalogID]; }

    void add(const Star& star) { _catalog.push_back(star); }
  
    Catalog(void);
    ~Catalog();
};

//-----------------------------------------------------------------------------

