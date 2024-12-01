#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

Scene::Scene(GameResources& game)
    : _game(game),
      _run_state(RunState::Running),
      _draw_state(DrawState::Solid) {}

void Scene::on_activation() {
    set_run_state(Scene::RunState::Running);
}

}  // namespace VVipers
