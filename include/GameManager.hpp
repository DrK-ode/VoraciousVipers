#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "GameWindow.hpp"
#include "Game.hpp"

class GameManager{
  public:
    GameManager();
    ~GameManager();
    void startGame();

  private:
    GameWindow* m_window;
    Game* m_game;
};


#endif