
// Timer.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <chrono>

// 3rdPartyLibs

// Caelum
#include "Export.h"
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes

class Timer
{
  // Definition
  private:
  protected:
  public:

  // Members
  private:
  protected:
    std::chrono::high_resolution_clock::time_point   _sT;
    std::chrono::high_resolution_clock::time_point   _eT;
    bool                                             _isTiming;

  public:

  // Methods
  private:
  protected:
  public:
    EXPORT bool  isTiming(void)
    { return _isTiming; }

    EXPORT void  start(void)
    {
      _sT         = std::chrono::high_resolution_clock::now(); 
      _isTiming  = true;
    }

    EXPORT void  stop(void)
    { 
      _eT        = std::chrono::high_resolution_clock::now(); 
      _isTiming = false;
    }

    EXPORT void reset(void)
    {
      _sT       = std::chrono::high_resolution_clock::now(); 
      _eT       = _sT;
      _isTiming = false;
    }

    EXPORT double seconds(void)
    {
    std::chrono::high_resolution_clock::time_point    eT(_isTiming ? std::chrono::high_resolution_clock::now() : _eT);
    std::chrono::duration<double>                     tD(std::chrono::duration<double>(eT - _sT));

      return tD.count();
    }

    EXPORT Timer(void) : _sT(std::chrono::high_resolution_clock::now()),
                          _eT(_sT),
                          _isTiming(true)
    {}   
};  

//-----------------------------------------------------------------------------
