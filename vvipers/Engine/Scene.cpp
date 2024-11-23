#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

Scene::Scene(Game& game)
    : _game(game),
      _run_state(RunState::Running),
      _draw_state(DrawState::Solid),
      _transition_state(TransitionState::Continue) {}

void Scene::on_activation() {
    set_run_state(Scene::RunState::Running);
    set_transition_state(TransitionState::Continue);
}

}  // namespace VVipers
