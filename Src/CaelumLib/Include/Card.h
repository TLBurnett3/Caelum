
// Card.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <vector>
#include <string>

// 3rdPartyLibs

// Caelum

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes

class Card
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
  protected:
    std::string _commonName;
    std::string _designation;
    double      _raDeg;
    double      _decDeg;
  public:   

  // Methods
  private:
  protected:
  public:
  
    Card(void);
    ~Card();
};


typedef std::vector<Card> StarIndex;
//-----------------------------------------------------------------------------

