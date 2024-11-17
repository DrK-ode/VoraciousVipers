#pragma once

#include <deque>
#include <memory>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/Scene.hpp>

namespace VVipers {

class Engine {
  public:
    Engine(std::unique_ptr<Game> game);
    void set_default_scene(std::shared_ptr<Scene> defaultScene) {
        _defaultScene = std::move(defaultScene);
    }
    void load_scene( std::shared_ptr<Scene> scene ){ scene_stack().push_back(std::move(scene));}
    void start_game();

  private:
    void draw();
    void game_loop(double fps);
    void process_events(Scene* scene);
    void scene_selection();
    void update(Time elapsedTime);
    std::deque<std::shared_ptr<Scene>>& scene_stack() {return _game->_scenes;}
    sf::RenderWindow& window() {return _game->_window;}

    std::unique_ptr<Game> _game;
    std::shared_ptr<Scene> _defaultScene;
};

}  // namespace VVipers