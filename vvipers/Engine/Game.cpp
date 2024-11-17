#include <SFML/Window/Window.hpp>
#include <memory>
#include <vvipers/Engine/ColorPalette.hpp>
#include <vvipers/Engine/Engine.hpp>
#include <vvipers/Engine/FontFileLoader.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/TextureFileLoader.hpp>

namespace VVipers {

Game::Game(std::unique_ptr<const OptionsProvider> options, Engine* engine)
    : _engine(engine),
      _options_provider(std::move(options)),
      _font_provider(std::make_unique<FontFileLoader>(*_options_provider)),
      _texture_provider(
          std::make_unique<TextureFileLoader>(*_options_provider)),
      _color_provider(std::make_unique<ColorPalette>(*_options_provider)) {}

void Game::set_grab_mouse(bool grabbed) { _engine->set_grab_mouse(grabbed); };
bool Game::is_mouse_grabbed() const { return _engine->is_mouse_grabbed(); }

const sf::RenderWindow& Game::window() const { return _engine->window(); }

}  // namespace VVipers