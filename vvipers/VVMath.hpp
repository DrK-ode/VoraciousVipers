#ifndef VVIPERS_VVMATH_HPP
#define VVIPERS_VVMATH_HPP

#include <cmath>

namespace VVipers {
const double pi = 3.14159265358979323846;
const double twopi = 2 * pi;
const double radPerDeg = pi / 180.f;
const double degPerRad = 180.f / pi;
inline double degToRad(double deg) { return deg * radPerDeg; }
inline double radToDeg(double rad) { return rad * degPerRad; }
inline double mod180Deg(double deg) {
    deg = std::fmod(deg, 360.f);
    if (deg > 180.f)
        deg -= 360.f;
    else if (deg < -180.f)
        deg += 360.f;
    return deg;
}
inline double modPiRad(double rad) {
    rad = std::fmod(rad, twopi);
    if (rad > pi)
        rad -= twopi;
    else if (rad < -pi)
        rad += twopi;
    return rad;
}

}  // namespace VVipers

#endif // VVIPERS_VVMATH_HPP
