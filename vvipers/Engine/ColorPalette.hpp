#ifndef VVIPERS_ENGINE_COLORPALETTE_HPP
#define VVIPERS_ENGINE_COLORPALETTE_HPP

#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <vvipers/Engine/Providers.hpp>

namespace VVipers {

class ColorPalette : public ColorProvider {
  public:
    ColorPalette(const OptionsProvider& options);
    sf::Color getColor(size_t index) const override;

  private:
    std::vector<sf::Color> m_palette;
};

}  // namespace VVipers

#endif  // VVIPERS_ENGINE_COLORPALETTE_HPP
