#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/Engine/Scene.hpp>
#include "vvipers/Engine/WindowManager.hpp"
#include "vvipers/Engine/ColorPalette.hpp"
#include "vvipers/Engine/FontFileLoader.hpp"
#include "vvipers/Engine/TextureFileLoader.hpp"

namespace VVipers {

class GameResources {
  public:
    GameResources(std::unique_ptr<OptionsProvider> options);
    const ColorProvider& color_service() const { return _color_provider; }
    const FontProvider& font_service() const { return _font_provider; }
    OptionsProvider& options_service() const { return *_options_provider; }
    const TextureProvider& texture_service() const {
        return _texture_provider;
    }
    WindowManager& window_manager() {return _window_manager;}

  private:
    const std::unique_ptr<OptionsProvider> _options_provider;
    const FontFileLoader _font_provider;
    const TextureFileLoader _texture_provider;
    const ColorPalette _color_provider;
    WindowManager _window_manager;
};

}  // namespace VVipers
