#ifndef VVIPERS_ENGINE_ENGINE_HPP
#define VVIPERS_ENGINE_ENGINE_HPP

#include <deque>
#include <memory>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/Scene.hpp>

namespace VVipers {

class Engine {
  public:
    Engine(std::unique_ptr<Game> game);
    void setDefaultScene(std::shared_ptr<Scene> defaultScene) {
        m_defaultScene = std::move(defaultScene);
    }
    void loadScene( std::shared_ptr<Scene> scene ){ getSceneStack().push_back(std::move(scene));}
    void startGame();

  private:
    void draw();
    void gameLoop(double FPS);
    void processEvents(Scene* scene);
    void sceneSelection();
    void update(Time elapsedTime);
    std::deque<std::shared_ptr<Scene>>& getSceneStack() {return m_game->m_scenes;}
    sf::RenderWindow& getWindow() {return m_game->m_window;}

    std::unique_ptr<Game> m_game;
    std::shared_ptr<Scene> m_defaultScene;
};

}  // namespace VVipers

#endif  // VVIPERS_ENGINE_ENGINE_HPP
