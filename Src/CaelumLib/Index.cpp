
// Index.cpp 
// Thomas Burnett


//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// CRos
#include "Index.h"

//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// getStarCard
//-----------------------------------------------------------------------------
int Index::getStarCard(const std::string &line,Card &card) const
{
std::vector<std::string> tokens;
int                      rc = 0;

  if (tokenize(line,',',tokens) > 0)
  {
  }
  else
    rc = -1;

  return rc;
}


//-----------------------------------------------------------------------------
// loadStarCards
//-----------------------------------------------------------------------------
int Index::loadStarCards(Catalog& catalog) const 
{
std::ifstream file(_filePath);
int           rc = 0;

  if (file.is_open()) 
  {
  std::string line;
  size_t parsedCount = 0;
  size_t skippedCount = 0;

    while (std::getline(file, line)) 
    {
      if (!line.empty() && (line[0] != '#')) 
      {
      Card card;   

        if (getStarCard(line,card) == 0)
        {
      //    catalog.add(card);
          parsedCount++;
        }
        else
          skippedCount++;
      }
    }

    std::cout << "Successfully loaded " << parsedCount << " Star Cards ("
              << skippedCount << " entries skipped)." << std::endl;
  }
  else
  {
    std::cerr << "Error: Could not open Hipparcos file: " << _filePath << std::endl;
    rc = -1;
  }

  return rc;
}


//-----------------------------------------------------------------------------
// Index
//-----------------------------------------------------------------------------
Index::Index(const std::string &_filepath)  : Parser(_filepath)
{
}

//-----------------------------------------------------------------------------
// ~Index
//-----------------------------------------------------------------------------
Index::~Index()
{

}

