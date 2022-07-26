#ifndef VVIPERS_CONTROLLER_HPP
#define VVIPERS_CONTROLLER_HPP

#include <vvipers/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace VVipers {

struct SteeringCommand {
  bool enable = true;
  double turn = 0.;
  bool boost = false;
};

class Controller {
  public:
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
    MouseController(const sf::RenderWindow* window)
        : m_window(window) {}

  private:
    virtual SteeringCommand control() const override;

    const sf::RenderWindow* m_window;
};

}  // namespace VVipers
#endif // VVIPERS_CONTROLLER_HPP
