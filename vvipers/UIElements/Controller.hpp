#ifndef VVIPERS_UIELEMENTS_CONTROLLER_HPP
#define VVIPERS_UIELEMENTS_CONTROLLER_HPP

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
        sf::Keyboard::Key left;
        sf::Keyboard::Key right;
        sf::Keyboard::Key boost;
    };
    KeyboardController(const KeyboardControls& keys) : m_keys(keys) {}

  private:
    virtual SteeringCommand control() const override;
    KeyboardControls m_keys;
};

class MouseController : public Controller {
  public:
    MouseController(Game& game);
    ~MouseController() { m_game.set_grab_mouse(false); }

  private:
    virtual SteeringCommand control() const override;
    Game& m_game;
};

}  // namespace VVipers
#endif  // VVIPERS_UIELEMENTS_CONTROLLER_HPP
