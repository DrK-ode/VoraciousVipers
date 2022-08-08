#ifndef VVIPERS_VVMATH_HPP
#define VVIPERS_VVMATH_HPP

#include <cmath>
#include <random>
#include <limits>

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

typedef std::ranlux48_base generatorEngine_t;
class Random {
  public:
    static double getDouble() {
        static std::uniform_real_distribution<double> distribution(0., 1.);
        return distribution(m_generator);
    }
    // Both first and last inclusive
    static int getInt(int first = 0, int last = std::numeric_limits<int>::max() ) {
        return getDouble(first, last+1);
    }
    // First inclusive, last exclusive
    static double getDouble(double first, double last) {
        return first + getDouble() * (last - first);
    }

  private:
    static std::random_device m_randomDevice;
    static generatorEngine_t m_generator;
};
inline std::random_device Random::m_randomDevice;
inline generatorEngine_t Random::m_generator( Random::m_randomDevice() );

}  // namespace VVipers

#endif  // VVIPERS_VVMATH_HPP
