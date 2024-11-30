#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <vvipers/Utilities/Time.hpp>
#include "vvipers/GameElements/Observer.hpp"

namespace VVipers {

class Scene;
class GameResources;

class Scene : public sf::Drawable, public Observable, public Observer {
  public:
    enum class RunState { Running, Paused };
    enum class DrawState { Solid, Transparent, Skip };

    Scene(GameResources& game);
    DrawState draw_state() const { return _draw_state; }
    GameResources& game_resources() { return _game; }
    virtual void on_activation();
    RunState run_state() const { return _run_state; }
    void set_draw_state(DrawState state) { _draw_state = state; }
    void set_run_state(RunState state) { _run_state = state; }

  private:
    GameResources& _game;
    RunState _run_state;
    DrawState _draw_state;
};

}  // namespace VVipers
