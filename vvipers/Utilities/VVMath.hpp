#pragma once

#include <cmath>
#include <limits>
#include <random>

namespace VVipers {
const double pi = 3.14159265358979323846;
const double twopi = 2 * pi;
const double rad_per_deg = pi / 180.f;
const double deg_per_rad = 180.f / pi;
inline double rad_from_deg(double deg) { return deg * rad_per_deg; }
inline double deg_from_rad(double rad) { return rad * deg_per_rad; }
inline double mod_180_deg(double deg) {
    deg = std::fmod(deg, 360.f);
    if (deg > 180.f)
        deg -= 360.f;
    else if (deg < -180.f)
        deg += 360.f;
    return deg;
}
inline double mod_pi_rad(double rad) {
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
    static double random_double() {
        static std::uniform_real_distribution<double> distribution(0., 1.);
        return distribution(_generator);
    }
    // Both first and last inclusive
    static int random_int(int first = 0,
                          int last = std::numeric_limits<int>::max()) {
        return random_double(first, last + 1);
    }
    // First inclusive, last exclusive
    static double random_double(double first, double last) {
        return first + random_double() * (last - first);
    }

  private:
    static std::random_device _random_device;
    static generatorEngine_t _generator;
};
inline std::random_device Random::_random_device;
inline generatorEngine_t Random::_generator(Random::_random_device());

}  // namespace VVipers
