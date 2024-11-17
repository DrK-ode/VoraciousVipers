#include <memory>
#include <vvipers/Engine/ColorPalette.hpp>
#include <vvipers/Engine/FontFileLoader.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/TextureFileLoader.hpp>

namespace VVipers {

Game::Game(std::unique_ptr<const OptionsProvider> options)
    : _options_provider(std::move(options)),
      _font_provider(std::make_unique<FontFileLoader>(*_options_provider)),
      _texture_provider(
          std::make_unique<TextureFileLoader>(*_options_provider)),
      _color_provider(std::make_unique<ColorPalette>(*_options_provider)),
      _is_mouse_grabbed(false) {
    Vec2 windowSize = _options_provider->option_2d_vector("General/windowSize");
    _window.create(sf::VideoMode(windowSize.x, windowSize.y),
                    "VoraciousVipers");
}

void Game::set_grab_mouse(bool grabbed) {
    _is_mouse_grabbed = grabbed;
    _window.setMouseCursorGrabbed(grabbed);
    _window.setMouseCursorVisible(!grabbed);
}

}  // namespace VVipers