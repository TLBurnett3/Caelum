
// ColorLUT.h
// Thomas Burnett

#pragma once

//-----------------------------------------------------------------------------
// Includes
// System
#include <vector>

// 3rdPartyLibs
#include <opencv2/opencv.hpp>

// Caelum

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Classes

class ColorLUT
{
  // Defines
  private:
  protected:
    typedef std::vector<cv::Scalar> ColorLUTVec;
  public:

  // Members
  private:
  protected:
    ColorLUTVec _lut;
  public:   

  // Methods
  private:
  protected:
    cv::Scalar ColorLUT::bvToBGR(double bv);

  public:
    cv::Scalar const getColor(double bv) const
    {
    int index = static_cast<int>(std::round((bv + 0.40) * 100.0));

      index = std::clamp(index, 0, 240);

      return _lut[index];   
    }

    ColorLUT(void);
    ~ColorLUT();
};

//-----------------------------------------------------------------------------

