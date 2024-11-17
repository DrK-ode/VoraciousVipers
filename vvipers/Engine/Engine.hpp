#pragma once

#include <deque>
#include <memory>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/Scene.hpp>

namespace VVipers {

class Engine {
  public:
    Engine(std::unique_ptr<const OptionsProvider> options);
    Game* game() const { return _game.get(); }
    void set_default_scene(std::shared_ptr<Scene> defaultScene) {
        _defaultScene = std::move(defaultScene);
    }
    void load_scene(std::shared_ptr<Scene> scene) {
        _scenes.push_back(std::move(scene));
    }
    const Scene* scene(size_t index) const { return _scenes[index].get(); }
    void start_game();
    void set_grab_mouse(bool grabbed);
    bool is_mouse_grabbed() const { return _is_mouse_grabbed; }
    const sf::RenderWindow& window() const { return _window; }

  private:
    void draw();
    void game_loop(double fps);
    void process_events(Scene* scene);
    void scene_selection();
    void update(Time elapsedTime);

    // Scenes are owned by either this stack or other Scenes.
    std::deque<std::shared_ptr<Scene>> _scenes;
    std::unique_ptr<Game> _game;
    std::shared_ptr<Scene> _defaultScene;
    sf::RenderWindow _window;
    bool _is_mouse_grabbed;
};

}  // namespace VVipers