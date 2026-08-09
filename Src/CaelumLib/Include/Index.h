
// Index.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// Caelum
#include "Parser.h"
#include "Card.h"
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes


class Index : public Parser
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
    int Index::getStarCard(const std::string &line,Card &card) const;

  public:
    int Index::loadStarCards(Catalog& catalog) const;
  
    Index(const std::string &_filepath);
    ~Index();
};

//-----------------------------------------------------------------------------

