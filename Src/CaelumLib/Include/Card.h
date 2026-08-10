
// Card.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <unordered_map>
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
    const std::string commonName(void) const
    { return _commonName; }

    void setCommonName(const std::string &name)
    { _commonName = name; }

    void setDesignation(const std::string &des)
    { _designation = des; }

    void setCelestialCoordinates(const double raDeg,const double decDeg)
    { 
      _raDeg   = raDeg;
      _decDeg  = decDeg;
    }
  
    Card(void);
    ~Card();
};


typedef std::unordered_map<std::string,Card> StarIndex;
//-----------------------------------------------------------------------------

