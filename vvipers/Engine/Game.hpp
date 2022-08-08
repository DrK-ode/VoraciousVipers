#ifndef VVIPERS_GAME_HPP
#define VVIPERS_GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <deque>
#include <memory>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/Engine/Scene.hpp>

namespace VVipers {

class Engine;

/** Game holds the window and other single instance objects, e.g., managers. **/
using scenestack_t = std::deque<std::shared_ptr<Scene>>;
class Game {
  public:
    Game(std::unique_ptr<const OptionsProvider> options);

    const sf::RenderWindow& getWindow() const { return m_window; };
    const ColorProvider& getColorService() const { return *m_colorProvider; }
    const FontProvider& getFontService() const { return *m_fontProvider; }
    const OptionsProvider& getOptionsService() const {
        return *m_optionsProvider;
    }
    const TextureProvider& getTextureService() const {
        return *m_textureProvider;
    }
    size_t getSceneStackHeight() const {return m_scenes.size();}
    const Scene* getScene(size_t index) const {return m_scenes[index].get();}
    const scenestack_t& getSceneStack() const { return m_scenes; }
    void setGrabMouse(bool grabbed);
    bool isMouseGrabbed() const {return m_mouseGrabbed;}

  private:
    sf::RenderWindow m_window;
    scenestack_t m_scenes;
    const std::unique_ptr<const OptionsProvider> m_optionsProvider;
    const std::unique_ptr<const FontProvider> m_fontProvider;
    const std::unique_ptr<const TextureProvider> m_textureProvider;
    const std::unique_ptr<const ColorProvider> m_colorProvider;

    bool m_mouseGrabbed;

    friend class Engine;
};

}  // namespace VVipers

#endif  // VVIPERS_GAME_HPP
