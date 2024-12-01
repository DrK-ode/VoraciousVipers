#pragma once

#include <deque>
#include <memory>
#include <vvipers/Engine/GameResources.hpp>
#include <vvipers/Engine/Scene.hpp>

#include "vvipers/GameElements/GameEvent.hpp"
#include "vvipers/GameElements/Observer.hpp"

namespace VVipers {

class Engine : public Observable, public Observer {
  public:
    Engine(std::unique_ptr<GameResources>);
    void add_scene(const std::shared_ptr<Scene>&);
    GameResources& game_resources() const { return *_game_resources.get(); }
    void set_default_scene(std::shared_ptr<Scene> defaultScene) {
        _defaultScene = std::move(defaultScene);
    }
    void on_notify(const GameEvent&) override;
    void start_game();

  private:
    void draw();
    void game_loop(double fps);
    void pop_scene();
    void process_window_events();
    void process_scene_events();
    void update(Time elapsedTime);

    // Scenes are owned by either this stack or other Scenes.
    std::deque<std::shared_ptr<Scene>> _scenes;
    std::unique_ptr<GameResources> _game_resources;
    std::shared_ptr<Scene> _defaultScene;
    std::vector<SceneEvent> _scene_events;
};

}  // namespace VVipers