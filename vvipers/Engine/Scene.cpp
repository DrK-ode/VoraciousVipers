#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

Scene::Scene(Game& game)
    : m_game(game),
      m_sceneState(SceneState::Running),
      m_transitionState(TransitionState::Continue),
      m_isTransparent(false) {}

void Scene::onActivation() {
    setSceneState(Scene::SceneState::Running);
    setTransitionState(TransitionState::Continue);
}

}  // namespace VVipers
