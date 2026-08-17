
// Scope.cpp 
// Thomas Burnett


//---------------------------------------------------------------------
// Includes
// System
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cstdint>

// 3rdPartyLibs
#include <opencv2/opencv.hpp> 
#include <glm/gtc/constants.hpp>

// CRos
#include "Timer.h"
#include "Scope.h"

//---------------------------------------------------------------------

//---------------------------------------------------------------------
// Constants and Defines

constexpr double EARTH_SIDEREAL_DEG_PER_SEC = 360.0 / 86164.0905; // ~0.0041780746 deg/s
//---------------------------------------------------------------------



//-----------------------------------------------------------------------------
// visualizeStellarFlux
//-----------------------------------------------------------------------------
cv::Mat Scope::visualizeStellarFlux(const cv::Mat& inputImg, bool applyNonLinearStretch, float stretchFactor) 
{
    if (inputImg.empty()) {
        std::cerr << "[Error] Input image is empty!" << std::endl;
        return cv::Mat();
    }

    // 1. SAFELY HANDLE MULTI-CHANNEL DATA
    // Avoid inputImg.reshape(1) which changes matrix dimensions without converting colors.
    cv::Mat singleChannel;
    if (inputImg.channels() == 3) {
        cv::cvtColor(inputImg, singleChannel, cv::COLOR_BGR2GRAY);
    } else if (inputImg.channels() == 4) {
        cv::cvtColor(inputImg, singleChannel, cv::COLOR_BGRA2GRAY);
    } else {
        singleChannel = inputImg.clone();
    }

    // 2. CONVERT TO FLOATING POINT (CV_32F) TO PREVENT INTEGER TRUNCATION
    cv::Mat fluxFloat;
    singleChannel.convertTo(fluxFloat, CV_32F);

    // 3. GET FULL DYNAMIC RANGE (MIN & MAX)
    double minVal, maxVal;
    cv::minMaxLoc(fluxFloat, &minVal, &maxVal);

    // Guard against a uniform or zero-variance image
    if (maxVal <= minVal) {
        return cv::Mat::zeros(fluxFloat.size(), CV_8UC1);
    }

    // 4. SUBTRACT MINVAL TO SET BACKGROUND BASELINE TO 0.0
    cv::Mat normalized = fluxFloat - static_cast<float>(minVal);
    float adjustedMax = static_cast<float>(maxVal - minVal);

    // 5. OPTIONAL NON-LINEAR STRETCH (ASINH)
    // Brings thousands of faint background stars into range without saturating bright peaks
    if (applyNonLinearStretch) {
        // Normalize linear flux to [0.0, 1.0] first
        normalized /= adjustedMax;

        // Apply asinh transform: asinh(x * factor)
        normalized.forEach<float>([&stretchFactor](float& pixel, const int* /*idx*/) {
            pixel = std::asinh(pixel * stretchFactor);
        });

        // Re-find min/max post-stretch to map back to [0.0, 1.0]
        double sMin, sMax;
        cv::minMaxLoc(normalized, &sMin, &sMax);
        if (sMax > sMin) {
            normalized = (normalized - static_cast<float>(sMin)) / static_cast<float>(sMax - sMin);
        }
    } else {
        // Standard linear scaling using full dynamic range [minVal, maxVal]
        normalized /= adjustedMax;
    }

    // 6. CONVERT TO 8-BIT DISPLAY MATRIX (CV_8UC1 [0, 255])
    cv::Mat visual8U;
    normalized.convertTo(visual8U, CV_8UC1, 255.0);

    return visual8U;
}

//-----------------------------------------------------------------------------
// visualizeStellarFlux
//-----------------------------------------------------------------------------
cv::Mat Scope::visualizeStellarFlux(const cv::Mat& rawSensorImage, float stretchScale)
{
  if (rawSensorImage.empty()) {
        return cv::Mat();
    }

    // 1. Convert BGR electron counts to single-channel total flux
    cv::Mat grayFlux;
    if (rawSensorImage.channels() == 3) {
        cv::cvtColor(rawSensorImage, grayFlux, cv::COLOR_BGR2GRAY);
    } else {
        grayFlux = rawSensorImage.clone();
    }

    // 2. Ensure we are operating in float space (CV_32F)
    cv::Mat fluxFloat;
    grayFlux.convertTo(fluxFloat, CV_32F);

    // 3. Non-linear stretch: log(1 + flux * scale)
    // Decompresses the magnitude scale so faint (1-10 e-) stars become visible 
    // alongside bright (>1e5 e-) stars.
    cv::Mat stretched;
    cv::log(1.0f + fluxFloat * stretchScale, stretched);

    // 4. Min/Max Normalize to [0.0, 1.0]
    double minVal, maxVal;
    cv::minMaxLoc(stretched, &minVal, &maxVal);

    if (maxVal <= minVal) {
        return cv::Mat::zeros(rawSensorImage.size(), CV_8UC1);
    }

    // 5. Convert to 8-bit display image (CV_8UC1 [0-255])
    cv::Mat displayImage;
    stretched.convertTo(displayImage, CV_8UC1, 255.0 / (maxVal - minVal), -minVal * (255.0 / (maxVal - minVal)));

    return displayImage;
}





//-----------------------------------------------------------------------------
// track2
// Calculates the precise 2D bin search space given telescope direction and FOV
//-----------------------------------------------------------------------------
void Scope::getBinSearchSpace(BinSearchSpace &space,
                              const double center_ra, const double center_dec, 
                              const double fov_radius_deg) 
{
  // 1. Calculate Declination Bin Bounds (clamped to [-90, +90])
  double min_dec = std::clamp(center_dec - fov_radius_deg, -90.0, 90.0);
  double max_dec = std::clamp(center_dec + fov_radius_deg, -90.0, 90.0);

  space.min_dec_bin = static_cast<int>(std::floor(min_dec + 90.0f));
  space.max_dec_bin = static_cast<int>(std::floor(max_dec + 90.0f));
  space.min_dec_bin = std::clamp(space.min_dec_bin, 0, 179);
  space.max_dec_bin = std::clamp(space.max_dec_bin, 0, 179);

  // 2. Check if the FOV overlaps either Celestial Pole
  if ((space.max_dec_bin == 179) || (center_dec + fov_radius_deg >= 90.0f)) 
  {
    space.touches_north_pole = true;
  }
  if ((space.min_dec_bin == 0) || (center_dec - fov_radius_deg <= -90.0f)) 
  {
    space.touches_south_pole = true;
  }

  constexpr double DEG_TO_RAD = 3.14159265358979323846 / 180.0;

  // 3. Compute RA bin spans for each declination row
  for (int dec_bin = space.min_dec_bin; dec_bin <= space.max_dec_bin; ++dec_bin) 
  {
    // RULE A: The North Polar Cap (bin 179) and South Polar Cap (bin 0) MUST span all 360 RA bins
    if (space.touches_north_pole || space.touches_south_pole) 
    {
      space.dec_row_spans.push_back({dec_bin, {{0, 359}}});
      continue;
    }

    // RULE B: Compute tight 1 / cos(dec) RA expansion for non-polar rows
    float dec_deg = static_cast<float>(dec_bin) - 90.0f;
        
    // Take the edge of the 1-degree bin closest to the pole for safe coverage
    float max_abs_dec = std::max(std::abs(dec_deg), std::abs(dec_deg + 1.0f));
        
    float cos_dec = std::cos(max_abs_dec * DEG_TO_RAD);
    float delta_ra = (cos_dec > 1e-4f) ? (fov_radius_deg / cos_dec) : 180.0f;

    // If RA expansion exceeds 180 degrees in either direction, sample the entire ring
    if (delta_ra >= 180.0f) {
        space.dec_row_spans.push_back({dec_bin, {{0, 359}}});
        continue;
    }

    // RULE C: Handle RA bounds & 0/360 boundary wrapping
    float min_ra = center_ra - delta_ra;
    float max_ra = center_ra + delta_ra;

    auto wrapRA = [](float ra) {
        float wrapped = std::fmod(ra, 360.0f);
        return (wrapped < 0.0f) ? wrapped + 360.0f : wrapped;
    };

    int min_ra_bin = static_cast<int>(std::floor(wrapRA(min_ra)));
    int max_ra_bin = static_cast<int>(std::floor(wrapRA(max_ra)));

    if (min_ra < 0.0f || max_ra >= 360.0f) {
        // Crossed 0/360 meridian: split into two spans [min_ra..359] and [0..max_ra]
        space.dec_row_spans.push_back({dec_bin, {{min_ra_bin, 359}, {0, max_ra_bin}}});
    } else {
        space.dec_row_spans.push_back({dec_bin, {{min_ra_bin, max_ra_bin}}});
    }
  }
}


//-----------------------------------------------------------------------------
// track
//-----------------------------------------------------------------------------
int Scope::track(Catalog &catalog,
                  const char *pStr, const double ra, const double dec,
                  const double gmstDegrees,const double exposureTime,
                  const uint32_t nFrames,ImageLst &imageLst,const bool display)
{
double    curLat    = _spTracker->getLatitude();
double    curLong   = _spTracker->getLongitude();
double    curLST    = gmstDegrees + curLong;
glm::vec3 vWorldUp  = glm::vec3(0.0f, 0.0f, 1.0f); // Celestial North Pole
glm::vec3 vDir      = geographicToEuclidean(ra,dec);
BinSearchSpace      space;

  getBinSearchSpace(space,ra,dec,_spOTA->fovDeg());

  for (uint32_t i = 0; i < nFrames; ++i)
  {
  cv::Mat image  = cv::Mat::zeros(_spSensor->imageHeight(),_spSensor->imageWidth(), CV_32FC3);
  Timer   timer;

    timer.start();

    curLST += exposureTime * EARTH_SIDEREAL_DEG_PER_SEC;

    double LST = normalize360(curLST);

    glm::vec3 vPos   = geographicToEuclidean(LST,dec);
    glm::vec3 vRight = glm::normalize(glm::cross(vDir, vPos));  // vPos is for altaz mount, vWorldUp is for equatorial mount
    glm::vec3 vUp    = glm::cross(vRight, vDir); 


    for (const auto& [dec_bin, ra_spans] : space.dec_row_spans) 
    {
      for (const auto& span : ra_spans) 
      {
        for (int ra_bin = span.min_ra; ra_bin <= span.max_ra; ++ra_bin) 
        {
        const Region *pRegion = catalog.region(ra_bin,dec_bin);
          
          for (size_t j = 0; j < pRegion->fieldSize(); ++j)
          {
          const Star *pStar = pRegion->getStar(j);
          glm::vec3  vStar  = celestialToEuclidean(*pStar);         

            double zCam = glm::dot(vStar,vDir);

            if (zCam > 0)
            {       
            // Project star onto Camera Right (x) and Up (y) axes
            double    xCam       = glm::dot(vStar, vRight);   // Horizontal displacement
            double    yCam       = glm::dot(vStar, vUp);      // Vertical displacement
            // Project star onto Camera Right (x) and Up (y) axes
            double    xTangent   = xCam / zCam;
            double    yTangent   = yCam / zCam;

              _spSensor->render(*pStar,exposureTime,xTangent,yTangent,image);
            }
          }
        }
      }
    }

    timer.stop();

    std::cout << pStr << " Frame: " << i << " " << timer.seconds() << std::endl;

    if (display)
    {
      if (1)
      {
      cv::Mat visualFrame = visualizeStellarFlux(image,500.0f);
  
        cv::imshow(pStr, visualFrame);
      }
      else
        cv::imshow(pStr,image);

      cv::waitKey(10);
    }

    imageLst.push_back(image);
  }

  return 0;
}


//----------------------------------------------------------------------------
// capture
//----------------------------------------------------------------------------
int Scope::captureImageSet(Catalog& catalog, const char *pTargetName, 
             const double ra, const double dec, 
             const double currentGMST, const double exposureTime, 
             const int numFrames,const bool display)
{
ImageLst imageStk;
int       rc = 0;

  rc = track(catalog, pTargetName, ra, dec, currentGMST, exposureTime, 
                numFrames, imageStk,display);

  return rc;
}


//----------------------------------------------------------------------------
// captureImageSet
//----------------------------------------------------------------------------
int Scope::captureImageSet(Catalog& catalog, const char *pTargetName, 
             const double currentGMST, const double exposureTime, 
             const int numFrames,const bool display)
{
ImageLst      imageStk;
int           rc = 0;
const Card    *pCard = catalog.getCardByName(pTargetName);

  if (pCard)
  {
  Timer timer;

    timer.start();
    {
      rc = track(catalog, pTargetName,
           pCard->raDeg(),pCard->decDeg(),
           currentGMST, exposureTime, numFrames, imageStk,display);
    }
    timer.stop();

    std::cout << timer.seconds() << " total seconds" << std::endl;
  }

  return rc;
}


//---------------------------------------------------------------------
// Scope
//---------------------------------------------------------------------
Scope::Scope(SpTracker& spTracker, SpOTA& spOTA, SpSensor& spSensor) : _spTracker(spTracker),
                                                                       _spOTA(spOTA),
                                                                       _spSensor(spSensor)
{
}


//---------------------------------------------------------------------
// ~Scope
//---------------------------------------------------------------------
Scope::~Scope()
{

}










/*


//-----------------------------------------------------------------------------
// track
//-----------------------------------------------------------------------------
int Scope::track(Catalog &catalog,
                  const char *pStr, const double ra, const double dec,
                  const double gmstDegrees,const double exposureTime,
                  const uint32_t nFrames,ImageLst &imageLst)
{
double    curLat    = _spTracker->getLatitude();
double    curLong   = _spTracker->getLongitude();
double    curLST    = gmstDegrees + curLong;
glm::vec3 vWorldUp  = glm::vec3(0.0f, 0.0f, 1.0f); // Celestial North Pole
glm::vec3 vDir      = geographicToEuclidean(ra,dec);

  for (uint32_t i = 0; i < nFrames; ++i)
  {
  cv::Mat image  = cv::Mat::zeros(_spSensor->imageHeight(),_spSensor->imageWidth(), CV_32FC3);
  Timer   timer;

    timer.start();

    curLST += exposureTime * EARTH_SIDEREAL_DEG_PER_SEC;

    double LST = normalize360(curLST);

    glm::vec3 vPos   = geographicToEuclidean(LST,dec);
    glm::vec3 vRight = glm::normalize(glm::cross(vDir, vPos));  // vPos is for altaz mount, vWorldUp is for equatorial mount
    glm::vec3 vUp    = glm::cross(vRight, vDir); 

    for (size_t j = 0; j < catalog.catalogSize(); ++j)
    {
    const Star *pStar = catalog.getStarByID(j);
    glm::vec3  vStar  = celestialToEuclidean(*pStar);         

      double zCam = glm::dot(vStar,vDir);

      if (zCam > 0)
      {       
      // Project star onto Camera Right (x) and Up (y) axes
      double    xCam       = glm::dot(vStar, vRight);   // Horizontal displacement
      double    yCam       = glm::dot(vStar, vUp);      // Vertical displacement
      // Project star onto Camera Right (x) and Up (y) axes
      double    xTangent   = xCam / zCam;
      double    yTangent   = yCam / zCam;

        _spSensor->render(*pStar,exposureTime,xTangent,yTangent,image);

      }
    }

    timer.stop();
   
    std::cout << pStr << " Frame: " << i << " " << timer.seconds() << std::endl;

    if (1)
    {
    cv::Mat visualFrame = visualizeStellarFlux(image,500.0f);

      cv::imshow(pStr,visualFrame);
    }
    else
      cv::imshow(pStr,image);

    cv::waitKey(100);

    imageLst.push_back(image);
  }

  return 0;
}

//----------------------------------------------------------------------------
// capture
//----------------------------------------------------------------------------
int Scope::capture(Catalog& catalog, const char *pTargetName, 
             const double currentGMST, const double exposureTime, 
             const int numFrames)
{
ImageLst      imageStk;
int           rc = 0;
const Card    *pCard = catalog.getCardByName(pTargetName);

  if (pCard)
  {
  Timer timer;

    timer.start();
    {
      rc = track(catalog, pTargetName,
           pCard->raDeg(),pCard->decDeg(),
           currentGMST, exposureTime, numFrames, imageStk);
    }
    timer.stop();

    std::cout << timer.seconds() << " total seconds" << std::endl;
  }

  return rc;
}
*/