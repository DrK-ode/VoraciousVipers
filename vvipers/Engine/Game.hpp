#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/Engine/Scene.hpp>

namespace VVipers {

class Engine;

class Game {
  public:
    Game(std::unique_ptr<OptionsProvider> options, Engine* engine);

    const ColorProvider& color_service() const { return *_color_provider; }
    const FontProvider& font_service() const { return *_font_provider; }
    OptionsProvider& options_service() const {
        return *_options_provider;
    }
    void set_grab_mouse(bool grabbed);
    bool is_mouse_grabbed() const;
    const TextureProvider& texture_service() const {
        return *_texture_provider;
    }
    const sf::RenderWindow& window() const;

  private:
    Engine* const _engine;
    const std::unique_ptr<OptionsProvider> _options_provider;
    const std::unique_ptr<const FontProvider> _font_provider;
    const std::unique_ptr<const TextureProvider> _texture_provider;
    const std::unique_ptr<const ColorProvider> _color_provider;
};

}  // namespace VVipers
