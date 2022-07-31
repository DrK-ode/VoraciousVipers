#ifndef VVIPERS_GAME_HPP
#define VVIPERS_GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <vvipers/Providers.hpp>

namespace VVipers {

class Game {
  public:
    Game(const OptionsProvider& options, const FontProvider& fonts,
         const TextureProvider& textures);

    sf::RenderWindow& getWindow() { return m_window; };
    const FontProvider& getFontService() const { return m_fontProvider; }
    const OptionsProvider& getOptionsService() const {
        return m_optionsProvider;
    }
    const TextureProvider& getTextureService() const {
        return m_textureProvider;
    }

    sf::RenderWindow m_window;
    const OptionsProvider& m_optionsProvider;
    const FontProvider& m_fontProvider;
    const TextureProvider& m_textureProvider;
};

}  // namespace VVipers

#endif  // VVIPERS_GAME_HPP
