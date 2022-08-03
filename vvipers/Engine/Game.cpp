#include <vvipers/Engine/Game.hpp>

namespace VVipers {

Game::Game(const OptionsProvider& options, const FontProvider& fonts,
           const TextureProvider& textures)
    : m_optionsProvider(options),
      m_fontProvider(fonts),
      m_textureProvider(textures), m_mouseGrabbed(false) {
    Vec2 windowSize = options.getOption2DVector("General/windowSize");
    m_window.create(sf::VideoMode(windowSize.x, windowSize.y),
                    "VoraciousVipers");
}

void Game::setGrabMouse(bool grabbed){
  m_mouseGrabbed = grabbed;
  m_window.setMouseCursorGrabbed(grabbed);
  m_window.setMouseCursorVisible(!grabbed);
}

}  // namespace VVipers