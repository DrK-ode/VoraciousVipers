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
inline std::tuple<double, double, double> fromHSLtoRGB(double H, double S,
                                                       double L) {
    H = std::fmod(H, 1.0);
    if (H < 0)
        H += 1.0;
    if (S < 0. or S > 1. or L < 0. or L > 1.)
        throw std::runtime_error("Invalid HSL values.");

    double c = (1. - std::abs(2. * L - 1.)) * S;
    double h = H * 6.;
    double x = c * (1. - std::abs(std::fmod(h, 2.) - 1.));

    double R = 0;
    double G = 0;
    double B = 0;
    if (h < 1) {
        R = c;
        G = x;
    } else if (h < 2) {
        R = x;
        G = c;
    } else if (h < 3) {
        G = c;
        B = x;
    } else if (h < 4) {
        G = x;
        B = c;
    } else if (h < 5) {
        R = x;
        B = c;
    } else {
        R = c;
        B = x;
    }

    double m = L - 0.5 * c;
    R += m;
    G += m;
    B += m;

    return {R, G, B};
}

// Following the conversion algorithm presented on Wikipedia
inline std::tuple<double, double, double> fromRGBtoHSL(double R, double G,
                                                       double B) {
    if (R < 0. or R > 1. or G < 0. or G > 1. or B < 0. or B > 1.)
        throw std::runtime_error("Invalid RGB values.");

    double xmax = std::max(std::max(R, G), B);
    double xmin = std::min(std::min(R, G), B);
    double c = xmax - xmin;
    double L = 0.5 * (xmax + xmin);

    double H;
    if (c == 0.)
        H = 0.;
    else if (xmax == R)
        H = (0 + (G - B) / c) / 6.;
    else if (xmax == G)
        H = (2 + (B - R) / c) / 6.;
    else if (xmax == B)
        H = (4 + (R - G) / c) / 6.;

    double S = 0;
    if (L > 0. and L < 1.)
        S = (xmax - L) / std::min(L, 1 - L);

    return {H, S, L};
}

inline sf::Color colorFromRGB(double R, double G, double B) {
    return sf::Color(std::min(uint32_t(255), uint32_t(256 * R)),
                     std::min(uint32_t(255), uint32_t(256 * G)),
                     std::min(uint32_t(255), uint32_t(256 * B)));
}

inline sf::Color colorFromHSL(double H, double S, double L) {
    auto [R, G, B] = fromHSLtoRGB(H, S, L);
    return colorFromRGB(R, G, B);
}

inline sf::Color blendColors(sf::Color c1, double f1, sf::Color c2, double f2) {
    return sf::Color(c1.r * f1 + c2.r * f2, c1.g * f1 + c2.g * f2,
                     c1.b * f1 + c2.b * f2);
}

}  // namespace VVipers

#endif  // VVIPERS_UTILITIES_VVCOLOR_HPP
