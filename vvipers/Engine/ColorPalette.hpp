#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <vvipers/Engine/Providers.hpp>

namespace VVipers {

class ColorPalette : public ColorProvider {
  public:
    ColorPalette(const OptionsProvider& options);
    sf::Color get_color(size_t index) const override;

  private:
    std::vector<sf::Color> _palette;
};

}  // namespace VVipers