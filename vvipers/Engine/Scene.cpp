#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

Scene::Scene(Game& game)
    : _game(game),
      _scene_state(SceneState::Running),
      _transition_state(TransitionState::Continue),
      _is_transparent(false) {}

void Scene::on_activation() {
    set_scene_state(Scene::SceneState::Running);
    set_transition_state(TransitionState::Continue);
}

}  // namespace VVipers
