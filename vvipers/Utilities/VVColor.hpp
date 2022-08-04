#ifndef VVIPERS_UTILITIES_VVCOLOR_HPP
#define VVIPERS_UTILITIES_VVCOLOR_HPP

#include <SFML/Graphics/Color.hpp>
#include <sstream>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

sf::Color stringToColor(const std::string& str) {
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

}  // namespace VVipers

#endif  // VVIPERS_UTILITIES_VVCOLOR_HPP
