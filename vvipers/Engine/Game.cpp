#include <vvipers/Engine/ColorPalette.hpp>
#include <vvipers/Engine/FontFileLoader.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/TextureFileLoader.hpp>

namespace VVipers {

Game::Game(std::unique_ptr<const OptionsProvider> options)
    : m_optionsProvider(std::move(options)),
      m_fontProvider(std::make_unique<FontFileLoader>(*m_optionsProvider)),
      m_textureProvider(
          std::make_unique<TextureFileLoader>(*m_optionsProvider)),
      m_colorProvider(std::make_unique<ColorPalette>(*m_optionsProvider)),
      m_mouseGrabbed(false) {
    Vec2 windowSize = m_optionsProvider->getOption2DVector("General/windowSize");
    m_window.create(sf::VideoMode(windowSize.x, windowSize.y),
                    "VoraciousVipers");
}

void Game::setGrabMouse(bool grabbed) {
    m_mouseGrabbed = grabbed;
    m_window.setMouseCursorGrabbed(grabbed);
    m_window.setMouseCursorVisible(!grabbed);
}

}  // namespace VVipers