#include <vvipers/Game.hpp>

namespace VVipers {

Game::Game(const OptionsProvider& options, const FontProvider& fonts,
           const TextureProvider& textures)
    : m_optionsProvider(options),
      m_fontProvider(fonts),
      m_textureProvider(textures) {
    Vec2 windowSize = options.getOption2DVector("General/windowSize");
    m_window.create(sf::VideoMode(windowSize.x, windowSize.y),
                    "VoraciousVipers");
}

}  // namespace VVipers