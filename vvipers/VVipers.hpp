#ifndef VVIPERS_VVIPERS_HPP
#define VVIPERS_VVIPERS_HPP

#include <vvipers/Game.hpp>
#include <vvipers/GameWindow.hpp>

namespace VVipers {

class VVipers {
  public:
    VVipers();
    ~VVipers();
    void startGame();

  private:
    GameWindow* m_window;
    Game* m_game;
};

}  // namespace VVipers

#endif