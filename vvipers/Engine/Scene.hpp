#ifndef VVIPERS_SCENES_SCENE_HPP
#define VVIPERS_SCENES_SCENE_HPP

#include <memory>
#include <vvipers/Utilities/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>

namespace VVipers {

class Scene;
class Game;
using scene_ptr = std::shared_ptr<Scene>;

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
    virtual void processEvent(const sf::Event& event) = 0;
    virtual void update(Time elapsedTime) = 0;
    // Go to a subScene, can be null if transition state is Return or Quit
    virtual scene_ptr makeTransition() { return scene_ptr(); }
    virtual void onReactivation();
    SceneState getSceneState() const { return m_sceneState; }
    void setSceneState(SceneState state) { m_sceneState = state; }
    TransitionState getTransitionState() const { return m_transitionState; }
    void setTransitionState(TransitionState state) {
        m_transitionState = state;
    }
    bool isTransparent() const { return m_isTransparent; }
    void setTransparent(bool transparent) { m_isTransparent = transparent; }
    Game& getGame() { return m_game; }

  private:
    Game& m_game;
    SceneState m_sceneState;
    TransitionState m_transitionState;
    bool m_isTransparent;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_SCENE_HPP
