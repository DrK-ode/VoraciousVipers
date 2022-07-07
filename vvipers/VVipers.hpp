#ifndef VVIPERS_VVIPERS_HPP
#define VVIPERS_VVIPERS_HPP

#include <vvipers/Game.hpp>
#include <vvipers/GameWindow.hpp>

namespace VVipers {

/**
 * Main manager class of the whole game.
 * This is where the render window is created and the events are polled. The
 * game loop is framrate limited.
 */
class VVipers {
  public:
    VVipers();
    ~VVipers();

    /**
     * Starts the game loop.
     * Currently hardcoded to 60 FPS.
     */
    void startGame();

  private:
    GameWindow* m_window;
    Game* m_game;
};

}  // namespace VVipers

#endif