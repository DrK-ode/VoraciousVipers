#include <vvipers/Engine/ColorPalette.hpp>
#include <vvipers/Utilities/VVColor.hpp>

namespace VVipers {

ColorPalette::ColorPalette(const OptionsProvider& options) {
    auto colorStrings = options.getOptionStringArray("General/colorPalette");
    for (auto& colorString : colorStrings) {
        m_palette.push_back(colorFromRGBString(colorString));
    }
    if (m_palette.empty())
        throw std::runtime_error("No colors found");
}

sf::Color ColorPalette::getColor(size_t index) const {
    return m_palette[index % m_palette.size()];
}

}  // namespace VVipers
