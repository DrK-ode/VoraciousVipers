#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

struct SteeringCommand {
    bool enable = true;
    double turn = 0.;
    bool boost = false;
};

class Controller {
  public:
    virtual ~Controller(){};
    virtual SteeringCommand control() const { return SteeringCommand(); }
};

class KeyboardController : public Controller {
  public:
    struct KeyboardControls {
        sf::Keyboard::Scancode left;
        sf::Keyboard::Scancode right;
        sf::Keyboard::Scancode boost;
    };
    KeyboardController(const KeyboardControls& keys) : _keys(keys) {}

  private:
    virtual SteeringCommand control() const override;
    KeyboardControls _keys;
};

class MouseController : public Controller {
  public:
    MouseController(Game& game);
    ~MouseController() { _game.set_grab_mouse(false); }

  private:
    virtual SteeringCommand control() const override;
    Game& _game;
};

}  // namespace VVipers
