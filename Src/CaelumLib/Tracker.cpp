
// Tracker.cpp 
// Thomas Burnett


//-----------------------------------------------------------------------------
// Includes
// System

// 3rdPartyLibs

// CRos
#include "Tracker.h"

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Tracker
//-----------------------------------------------------------------------------
Tracker::Tracker(const double longitude, const double latitude) : _longitude(longitude),
                                                                  _latitude(latitude)
{
}

//-----------------------------------------------------------------------------
// ~Tracker
//-----------------------------------------------------------------------------
Tracker::~Tracker()
{

}

/*

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <iomanip>
#include <glm/glm.hpp>

// ============================================================================
// Astronomy Helpers
// ============================================================================

// Wrap angle to [0, 360) degrees
inline double normalize360(double angle) {
    double res = std::fmod(angle, 360.0);
    return (res < 0.0) ? res + 360.0 : res;
}

// Compute Julian Date from std::chrono system clock
double getJulianDate(const std::chrono::system_clock::time_point& tp) {
    auto duration = tp.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    // 86400 seconds in a day; Unix epoch (Jan 1 1970) is Julian Date 2440587.5
    return 2440587.5 + (seconds / 86400.0);
}

// Compute GMST (degrees) from Julian Date using IAU-82 formula
double calculateGMST(double julianDate) {
    double jd0 = std::floor(julianDate - 0.5) + 0.5;
    double utHours = (julianDate - jd0) * 24.0;
    double T0 = (jd0 - 2451545.0) / 36525.0;

    double gmst = 100.46061837 
                + (36000.770053608 * T0) 
                + (0.000387933 * T0 * T0) 
                + (15.0410686 * utHours);

    return normalize360(gmst);
}

struct AltAz {
    double altDeg; // Altitude above horizon (0° to 90°)
    double azDeg;  // Azimuth from North (0° = N, 90° = E, 180° = S, 270° = W)
    bool isVisible;
};

// Calculate horizontal Alt/Az coordinates for a celestial target
AltAz calculateAltAz(double raDeg, double decDeg, double lstDeg, double latDeg) {
    double haDeg  = lstDeg - raDeg;
    
    double latRad = glm::radians(latDeg);
    double decRad = glm::radians(decDeg);
    double haRad  = glm::radians(haDeg);

    // Altitude calculation
    double sinAlt = std::sin(latRad) * std::sin(decRad) + 
                    std::cos(latRad) * std::cos(decRad) * std::cos(haRad);
    sinAlt = glm::clamp(sinAlt, -1.0, 1.0);
    double altRad = std::asin(sinAlt);

    // Azimuth calculation
    double cosAlt = std::cos(altRad);
    double cosAz = (std::sin(decRad) - std::sin(latRad) * sinAlt) / 
                   (std::cos(latRad) * cosAlt);
    cosAz = glm::clamp(cosAz, -1.0, 1.0);

    double azRad = std::acos(cosAz);
    if (std::sin(haRad) > 0.0) { // West of local meridian
        azRad = glm::two_pi<double>() - azRad;
    }

    AltAz result;
    result.altDeg = glm::degrees(altRad);
    result.azDeg  = glm::degrees(azRad);
    result.isVisible = (result.altDeg > 0.0);
    return result;
}

// ============================================================================
// Tracking Loop Implementation
// ============================================================================

class TelescopeTracker {
public:
    TelescopeTracker(double latitude, double longitude) 
        : _latitude(latitude), _longitude(longitude) {}

    void startTracking(double targetRaDeg, double targetDecDeg, double targetHz = 10.0) {
        _isTracking = true;

        // Target loop rate (e.g., 10 Hz = 100ms update interval)
        const auto loopInterval = std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::duration<double>(1.0 / targetHz)
        );

        auto previousTime = std::chrono::system_clock::now();
        
        // Initial state evaluation
        double jd = getJulianDate(previousTime);
        double lst = normalize360(calculateGMST(jd) + _longitude);
        AltAz currentAltAz = calculateAltAz(targetRaDeg, targetDecDeg, lst, _latitude);

        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Starting tracking loop for Target (RA: " << targetRaDeg 
                  << "°, Dec: " << targetDecDeg << "°)\n";
        std::cout << "Location: Austin, TX (Lat: " << _latitude 
                  << "°, Lon: " << _longitude << "°)\n";
        std::cout << "----------------------------------------------------------------------\n";

        while (_isTracking) {
            auto currentTime = std::chrono::system_clock::now();
            
            // 1. Calculate actual elapsed time delta (dt) in seconds
            std::chrono::duration<double> dtDuration = currentTime - previousTime;
            double dt = dtDuration.count();

            if (dt <= 0.0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }

            // 2. Compute current Local Sidereal Time
            jd = getJulianDate(currentTime);
            double gmst = calculateGMST(jd);
            lst = normalize360(gmst + _longitude);

            // 3. Compute updated Target Alt/Az
            AltAz newAltAz = calculateAltAz(targetRaDeg, targetDecDeg, lst, _latitude);

            // 4. Calculate required motor speeds (deg/sec)
            // Handle Azimuth 360°/0° boundary crossing
            double dAz = newAltAz.azDeg - currentAltAz.azDeg;
            if (dAz > 180.0)  dAz -= 360.0;
            if (dAz < -180.0) dAz += 360.0;

            double dAlt = newAltAz.altDeg - currentAltAz.altDeg;

            double azVelocity  = dAz / dt;   // deg/sec
            double altVelocity = dAlt / dt;  // deg/sec

            // 5. Log status / Pass velocities to hardware motor controllers
            std::cout << "LST: " << std::setw(8) << lst << "° | "
                      << "Alt: " << std::setw(7) << newAltAz.altDeg << "° | "
                      << "Az: "  << std::setw(8) << newAltAz.azDeg  << "° | "
                      << "v_Alt: " << std::setw(8) << (altVelocity * 3600.0) << " arcsec/s | "
                      << "v_Az: "  << std::setw(8) << (azVelocity  * 3600.0) << " arcsec/s\n";

            // Update state for next frame
            currentAltAz = newAltAz;
            previousTime = currentTime;

            // 6. Sleep to maintain steady target loop rate
            auto workTime = std::chrono::system_clock::now() - currentTime;
            if (workTime < loopInterval) {
                std::this_thread::sleep_for(loopInterval - workTime);
            }
        }
    }

    void stopTracking() {
        _isTracking = false;
    }

private:
    double _latitude;
    double _longitude;
    bool _isTracking = false;
};

// ============================================================================
// Main Execution
// ============================================================================

int main() {
    // Austin, Texas coordinates
    constexpr double AUSTIN_LAT = 30.2672;
    constexpr double AUSTIN_LON = -97.7431; // East-positive convention (-97.74° = West)

    // Target: Vega (RA: 18h 36m 56s = 279.2347°, Dec: +38.7837°)
    constexpr double VEGA_RA  = 279.2347;
    constexpr double VEGA_DEC = 38.7837;

    TelescopeTracker tracker(AUSTIN_LAT, AUSTIN_LON);

    // Run tracking loop at 10 Hz rate
    tracker.startTracking(VEGA_RA, VEGA_DEC, 10.0);

    return 0;
}*/