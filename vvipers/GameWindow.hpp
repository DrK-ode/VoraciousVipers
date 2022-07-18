#ifndef VVIPERS_GAMEWINDOW_HPP
#define VVIPERS_GAMEWINDOW_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <vvipers/Observer.hpp>

namespace VVipers {

class GameWindow : public sf::RenderWindow, public Observable {
  public:
    GameWindow();
    ~GameWindow();
    void processEvents();
};

}  // namespace VVipers

#endif