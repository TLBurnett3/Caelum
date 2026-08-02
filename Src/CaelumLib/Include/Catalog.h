
// Catalog.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// Caelum
#include "Scope.h"
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

    int Catalog::filter(Scope &scope, StarField &starField) const;

    void add(const Star& star) { _catalog.push_back(star); }
  
    Catalog(void);
    ~Catalog();
};

//-----------------------------------------------------------------------------

