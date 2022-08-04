#ifndef VVIPERS_UTILITIES_VVCOLOR_HPP
#define VVIPERS_UTILITIES_VVCOLOR_HPP

#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include <sstream>
#include <tuple>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

inline sf::Color colorFromRGBString(const std::string& str) {
    if (str.size() != 7 or str.at(0) != '#')
        throw std::runtime_error("Found invalid color string.");
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << str.substr(1);
    ss >> x;
    auto r = (x & 0xff0000) >> 16;
    auto g = (x & 0x00ff00) >> 8;
    auto b = (x & 0x0000ff) >> 0;
    return sf::Color(r, g, b);
}

// Following the conversion algorithm presented on Wikipedia
inline sf::Color colorFromHSL(double H, double S, double L) {
    H = std::fmod(H, 1.0);
    if (H < 0)
        H += 1.0;
    if (S < 0. or S > 1. or L < 0. or L > 1.)
        throw std::runtime_error("Invalid HSL input.");

    double c = (1. - std::abs(2. * L - 1.)) * S;
    double h = H * 6.;
    double x = c * (1. - std::abs(std::fmod(h, 2.) - 1.));

    double r = 0;
    double g = 0;
    double b = 0;
    if (h < 1) {
        r = c;
        g = x;
    } else if (h < 2) {
        r = x;
        g = c;
    } else if (h < 3) {
        g = c;
        b = x;
    } else if (h < 4) {
        g = x;
        b = c;
    } else if (h < 5) {
        r = x;
        b = c;
    } else {
        r = c;
        b = x;
    }

    double m = L - 0.5 * c;
    r += m;
    g += m;
    b += m;

    return sf::Color( uint8_t(255*r), uint8_t(255*g), uint8_t(255*b));
}

}  // namespace VVipers

#endif  // VVIPERS_UTILITIES_VVCOLOR_HPP
