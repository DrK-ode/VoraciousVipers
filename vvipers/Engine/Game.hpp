#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <deque>
#include <memory>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/Engine/Scene.hpp>

namespace VVipers {

class Engine;

class Game {
  public:
    Game(std::unique_ptr<const OptionsProvider> options);

    const sf::RenderWindow& window() const { return _window; };
    const ColorProvider& color_service() const { return *_color_provider; }
    const FontProvider& font_service() const { return *_font_provider; }
    const OptionsProvider& options_service() const {
        return *_options_provider;
    }
    const TextureProvider& texture_service() const {
        return *_texture_provider;
    }
    const Scene* scene(size_t index) const { return _scenes[index].get(); }
    const std::deque<std::shared_ptr<Scene>>& scene_stack() const {
        return _scenes;
    }
    void set_grab_mouse(bool grabbed);
    bool is_mouse_grabbed() const { return _is_mouse_grabbed; }

  private:
    sf::RenderWindow _window;
    // Scenes are owned by either this stack or other Scenes.
    std::deque<std::shared_ptr<Scene>> _scenes;
    const std::unique_ptr<const OptionsProvider> _options_provider;
    const std::unique_ptr<const FontProvider> _font_provider;
    const std::unique_ptr<const TextureProvider> _texture_provider;
    const std::unique_ptr<const ColorProvider> _color_provider;

    bool _is_mouse_grabbed;

    friend class Engine;
};

}  // namespace VVipers
