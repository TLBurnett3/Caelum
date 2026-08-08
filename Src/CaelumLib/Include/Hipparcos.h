#pragma once

// Hipparcos.h
// Thomas Burnett

//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// Caelum
#include "Parser.h"
//-----------------------------------------------------------------------------
 
//-----------------------------------------------------------------------------
// Classes
class Hipparcos : public Parser
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
  protected:
  public:

  // Methods
  private:
  protected:
  public:

    int getStar(const std::string& line, Star &star) const;
  
  public:

    virtual int loadStarCatalog(Catalog& catalog) const;   

    Hipparcos(const std::string &filepath);
    ~Hipparcos();

};
//-----------------------------------------------------------------------------