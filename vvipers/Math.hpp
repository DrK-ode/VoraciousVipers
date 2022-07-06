#ifndef VVIPERS_MATH_HPP
#define VVIPERS_MATH_HPP

#include <cmath>

namespace VVipers {
// TODO Change all floats to doubles
const float pi = 3.14159265358979323846;
const float twopi = 2 * pi;
const float radPerDeg = pi / 180.f;
const float degPerRad = 180.f / pi;
inline float degToRad(float deg) { return deg * radPerDeg; }
inline float radToDeg(float rad) { return rad * degPerRad; }
inline float mod180Deg(float deg) {
    deg = std::fmod(deg, 360.f);
    if (deg > 180.f)
        deg -= 360.f;
    else if (deg < -180.f)
        deg += 360.f;
    return deg;
}
inline float modPiRad(float rad) {
    rad = std::fmod(rad, twopi);
    if (rad > pi)
        rad -= twopi;
    else if (rad < -pi)
        rad += twopi;
    return rad;
}

}  // namespace VVipers

#endif