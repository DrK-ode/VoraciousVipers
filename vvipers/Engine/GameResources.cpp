#include "vvipers/Engine/GameResources.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>
#include <memory>
#include <vvipers/Engine/ColorPalette.hpp>
#include <vvipers/Engine/Engine.hpp>
#include <vvipers/Engine/FontFileLoader.hpp>
#include <vvipers/Engine/TextureFileLoader.hpp>

namespace VVipers {

GameResources::GameResources(std::unique_ptr<OptionsProvider> options)
    : _options_provider(std::move(options)),
      _font_provider(*_options_provider),
      _texture_provider(*_options_provider),
      _color_provider(*_options_provider),
      _window_manager(sf::Vector2u(
          _options_provider->option_2d_vector("General/windowSize"))) {}

}  // namespace VVipers