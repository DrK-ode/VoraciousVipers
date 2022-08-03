#ifndef VVIPERS_ENGINE_HPP
#define VVIPERS_ENGINE_HPP

#include <deque>
#include <memory>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/Scene.hpp>

namespace VVipers {

class Engine {
  public:
    Engine(std::unique_ptr<Game> game);
    void setDefaultScene(scene_ptr defaultScene) {
        m_defaultScene = defaultScene;
    }
    void loadScene( scene_ptr scene ){ getSceneStack().push_back(scene);}
    void startGame();

  private:
    void draw();
    void gameLoop(double FPS);
    void sceneSelection();
    void update(Time elapsedTime);
    scenestack_t& getSceneStack() {return m_game->m_scenes;}

    std::unique_ptr<Game> m_game;
    scene_ptr m_defaultScene;
};

}  // namespace VVipers

#endif  // VVIPERS_ENGINE_HPP
