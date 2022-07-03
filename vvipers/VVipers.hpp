#ifndef VVIPERS_VVIPERS_HPP
#define VVIPERS_VVIPERS_HPP

#include <vvipers/GameWindow.hpp>
#include <vvipers/Game.hpp>

class VVipers{
  public:
    VVipers();
    ~VVipers();
    void startGame();

  private:
    GameWindow* m_window;
    Game* m_game;
};


#endif