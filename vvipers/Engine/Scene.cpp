#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

Scene::Scene()
    : m_sceneState(SceneState::Running),
      m_transitionState(TransitionState::Continue),
      m_isTransparent(false) {}

void Scene::onReactivation() {
    setSceneState(Scene::SceneState::Running);
    setTransitionState(TransitionState::Continue);
}

}  // namespace VVipers
