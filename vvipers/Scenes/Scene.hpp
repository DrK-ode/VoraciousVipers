#ifndef VVIPERS_ENGINE_SCENE_HPP
#define VVIPERS_ENGINE_SCENE_HPP

#include <memory>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Scene;
using scene_ptr = std::shared_ptr<Scene>;

class Scene {
  public:
    enum class SceneState { Running, Paused };
    enum class TransitionState { Continue, Replace, Return, Spawn, Quit };
    // Game holds the window and other single instance objects, e.g., managers.
    Scene();
    virtual void draw() = 0;
    virtual void processEvents() = 0;
    virtual void update(Time elapsedTime) = 0;
    // Go to a subScene, can be null if transition state is Return or Quit
    virtual scene_ptr makeTransition() = 0;
    SceneState getSceneState() const { return m_sceneState; }
    void setSceneState(SceneState state) { m_sceneState = state; }
    TransitionState getTransitionState() const { return m_transitionState; }
    void setTransitionState(TransitionState state) {
        m_transitionState = state;
    }
    bool isTransparent() const { return m_isTransparent; }
    void setTransparent(bool transparent) { m_isTransparent = transparent; }

  private:
    SceneState m_sceneState;
    TransitionState m_transitionState;
    bool m_isTransparent;
};

}  // namespace VVipers

#endif  // VVIPERS_ENGINE_SCENE_HPP
