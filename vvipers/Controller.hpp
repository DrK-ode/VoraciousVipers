#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <vvipers/GameEvent.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/Time.hpp>

namespace VVipers {

class Controller : public Observable, public Observer {
  public:
    virtual void onNotify(const GameEvent* event);

  protected:
    virtual void update(const Time& elapsedTime) {}
};

class KeyboardController : public Controller {
  public:
    KeyboardController(sf::Keyboard::Key left, sf::Keyboard::Key right)
        : m_left(left), m_right(right) {}

  private:
    virtual void update( const Time& elapsedTime) override;
    sf::Keyboard::Key m_left;
    sf::Keyboard::Key m_right;
};

class ControllerGoingInCircles : public Controller {
  public:
    ControllerGoingInCircles(double da) : m_da(da) {}

  private:
  virtual void update( const Time& elapsedTime) override;
    double m_da;
};

}  // namespace VVipers
#endif