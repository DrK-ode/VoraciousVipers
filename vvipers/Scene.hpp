#ifndef VVIPERS_SCENE_HPP
#define VVIPERS_SCENE_HPP

#include <memory>
#include <vvipers/Time.hpp>

namespace VVipers {

class Scene {
  public:
    enum class SceneState { Running, Paused };
    enum class TransitionState { Continue, Replace, Return, Spawn, Quit };
    // Game holds the window and other single instance objects, e.g., managers.
    Scene();
    virtual void draw() = 0;
    virtual void processEvents() = 0;
    virtual void update(Time elapsedTime) = 0;
    // Go to a subScene, if null just pop this scene
    virtual std::unique_ptr<Scene> getTransition() {
        return std::unique_ptr<Scene>();
    }
    SceneState getSceneState() const { return m_sceneState; }
    void setSceneState(SceneState state) { m_sceneState = state; }
    TransitionState getTransitionState() const { return m_transitionState; }
    void setTransitionState(TransitionState state) {
        m_transitionState = state;
    }

  private:
    SceneState m_sceneState;
    TransitionState m_transitionState;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENE_HPP
