#include <vvipers/Engine/ColorPalette.hpp>
#include <vvipers/Utilities/VVColor.hpp>

namespace VVipers {

ColorPalette::ColorPalette(const OptionsProvider& options) {
    auto colorStrings = options.option_string_array("General/colorPalette");
    for (auto& colorString : colorStrings) {
        _palette.push_back(color_from_rgb_string(colorString));
    }
    if (_palette.empty())
        throw std::runtime_error("No colors found");
}

sf::Color ColorPalette::get_color(size_t index) const {
    return _palette[index % _palette.size()];
}

}  // namespace VVipers
