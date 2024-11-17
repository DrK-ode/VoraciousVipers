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
    /** Whether to update the Scene or not (regardless if it is on the top of
     * the stack) **/
    enum class SceneState { Running, Paused };
    /** Different ways of transitioning **/
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
    virtual void process_event(const sf::Event& event) = 0;
    virtual void update(Time elapsedTime) = 0;
    // Go to a subScene, can be null if transition state is Return or Quit
    virtual std::shared_ptr<Scene> make_transition() {
        return std::shared_ptr<Scene>();
    }
    virtual void on_activation();
    SceneState scene_state() const { return _scene_state; }
    void set_scene_state(SceneState state) { _scene_state = state; }
    TransitionState transition_state() const { return _transition_state; }
    void set_transition_state(TransitionState state) {
        _transition_state = state;
    }
    bool is_transparent() const { return _is_transparent; }
    void set_transparency(bool transparent) { _is_transparent = transparent; }
    Game& game() { return _game; }

  private:
    Game& _game;
    SceneState _scene_state;
    TransitionState _transition_state;
    bool _is_transparent;
};

}  // namespace VVipers
