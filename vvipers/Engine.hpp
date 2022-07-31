#ifndef VVIPERS_ENGINE_HPP
#define VVIPERS_ENGINE_HPP

#include <deque>
#include <vvipers/Scene.hpp>
#include <vvipers/Game.hpp>

namespace VVipers {

class Engine {
  public:
    Engine(const OptionsProvider& options, const FontProvider& fonts, const TextureProvider& textures);
    void startGame();

  private:
    void draw();
    void gameLoop(double FPS);
    void loadFirstScene();
    void sceneSelection();
    void update(Time elapsedTime);

    Game m_game;
    std::deque< std::unique_ptr<Scene> > m_scenes;
};

}  // namespace VVipers

#endif  // VVIPERS_ENGINE_HPP
