#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <SFML/Graphics.hpp>

class GameWindow : public sf::RenderWindow {
  public:
    GameWindow();
    ~GameWindow();
};

#endif