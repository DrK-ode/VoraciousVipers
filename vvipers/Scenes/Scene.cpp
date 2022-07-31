#include <vvipers/Scenes/Scene.hpp>

namespace VVipers {

Scene::Scene()
    : m_sceneState(SceneState::Running),
      m_transitionState(TransitionState::Continue),
      m_isTransparent(false) {}

}  // namespace VVipers
