
// Tycho2.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System


// 3rdPartyLibs

// Caelum
#include "Parser.h"

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes

class Tycho2 : public Parser
{
  // Defines
  private:
  protected:
  public:

  // Members
  private:
  protected:
    std::string _filepath;
  public:   

  // Methods
  private:
  protected:
    // Pack (TYC1, TYC2, TYC3) into a single uint32_t
    inline uint32_t packTychoID(uint16_t tyc1, uint16_t tyc2, uint16_t tyc3) const
    {
        return (static_cast<uint32_t>(tyc1) << 17) |
               (static_cast<uint32_t>(tyc2) << 3)  |
               (static_cast<uint32_t>(tyc3) & 0x07);
    }

    // Unpack uint32_t back to original components
    inline void unpackTychoID(uint32_t id, uint16_t& tyc1, uint16_t& tyc2, uint8_t& tyc3) {
        tyc1 = static_cast<uint16_t>((id >> 17) & 0x3FFF); // Top 14 bits
        tyc2 = static_cast<uint16_t>((id >> 3)  & 0x3FFF); // Middle 14 bits
        tyc3 = static_cast<uint8_t>(id & 0x07);            // Bottom 3 bits
    }

    int Tycho2::getStar(const std::string &line,Star &star) const;

    int loadTypcho2(Catalog& catalog,const std::filesystem::path& fileName) const;

  public:
    virtual int loadStarCatalog(Catalog& catalog) const;
  
    Tycho2(const std::string &filepath);
    ~Tycho2();
};

//-----------------------------------------------------------------------------

