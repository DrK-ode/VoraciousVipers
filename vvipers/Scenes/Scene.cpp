#include <vvipers/Scenes/Scene.hpp>

namespace VVipers
{

Scene::Scene() : m_sceneState(SceneState::Running), m_transitionState(TransitionState::Continue) {}
    
} // namespace VVipers
