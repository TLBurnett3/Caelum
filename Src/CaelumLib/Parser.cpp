
// Parser.cpp 
// Thomas Burnett


//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// CRos
#include "Parser.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// tokenize
//-----------------------------------------------------------------------------
size_t Parser::tokenize(const std::string& input, char delimiter,std::vector<std::string> &tokens) const
{
std::istringstream stream(input);
std::string token;

  while (std::getline(stream, token, delimiter)) 
    tokens.push_back(token);
 
  return tokens.size();
}

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------
Parser::Parser( const std::string &filePath) :_filePath(filePath)
{
}

//-----------------------------------------------------------------------------
// ~Parser
//-----------------------------------------------------------------------------
Parser::~Parser()
{

}

