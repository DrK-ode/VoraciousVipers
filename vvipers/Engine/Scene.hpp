#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Scene;
class Game;

class Scene : public sf::Drawable {
  public:
    enum class RunState { Running, Paused };
    enum class DrawState { Solid, Transparent, Skip };
    enum class TransitionState {
        Clear,     // Pop the whole stack and push the new Scene.
        Default,   // Pop the whole stack and push the default Scene.
        Continue,  // No transition, keep going.
        JumpTo,    // Pop until the chosen Scene is reached.
        Replace,   // Pop this Scene and push the new Scene.
        Return,    // Pop this Scene.
        Spawn,     // Push the new Scene.
        Quit       // Pop all Scenes.
    };

    Scene(Game& game);
    DrawState draw_state() const { return _draw_state; }
    Game& game() { return _game; }
    // Go to a subScene, can be null if transition state is Return or Quit
    virtual std::shared_ptr<Scene> make_transition() {
        return std::shared_ptr<Scene>();
    }
    virtual void on_activation();
    virtual void process_event(const sf::Event& event) = 0;
    RunState run_state() const { return _run_state; }
    void set_draw_state(DrawState state) { _draw_state = state; }
    void set_run_state(RunState state) { _run_state = state; }
    TransitionState transition_state() const { return _transition_state; }
    void set_transition_state(TransitionState state) {
        _transition_state = state;
    }
    virtual void update(Time elapsedTime) = 0;

  private:
    Game& _game;
    RunState _run_state;
    DrawState _draw_state;
    TransitionState _transition_state;
};

}  // namespace VVipers
