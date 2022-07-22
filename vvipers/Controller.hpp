#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <vvipers/GameEvent.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace VVipers {

class Controller : public Observable, public Observer {
  public:
    virtual void onNotify(const GameEvent* event);

  protected:
    virtual void update(const Time& elapsedTime) {}
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
    virtual void update(const Time& elapsedTime) override;
    KeyboardControls m_keys;
};

class MouseController : public Controller {
  public:
    MouseController(const sf::RenderWindow* window)
        : m_window(window) {}

  private:
    virtual void update(const Time& elapsedTime) override;

    const sf::RenderWindow* m_window;
};

class ControllerGoingInCircles : public Controller {
  public:
    ControllerGoingInCircles(double da) : m_da(da) {}

  private:
    virtual void update(const Time& elapsedTime) override;
    double m_da;
};

}  // namespace VVipers
#endif