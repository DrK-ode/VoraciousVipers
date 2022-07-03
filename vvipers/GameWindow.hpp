#ifndef VVIPERS_GAMEWINDOW_HPP
#define VVIPERS_GAMEWINDOW_HPP

#include <SFML/Graphics.hpp>

class GameWindow : public sf::RenderWindow {
  public:
    GameWindow();
    ~GameWindow();
};

#endif