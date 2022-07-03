#ifndef VVIPERS_GAMEWINDOW_HPP
#define VVIPERS_GAMEWINDOW_HPP

#include <SFML/Graphics.hpp>

namespace VVipers {

class GameWindow : public sf::RenderWindow {
  public:
    GameWindow();
    ~GameWindow();
};

}  // namespace VVipers

#endif