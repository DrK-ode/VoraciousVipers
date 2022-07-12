#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <vvipers/Observer.hpp>
#include <vvipers/Time.hpp>

namespace VVipers {

class Controller : public Observable {
  public:
    Controller() : m_connected(false) {}
    virtual void onUpdate(const Time& elapsedTime){};
    virtual void onKeyboard(){};
    virtual void onMouse(){};

    static const Controller dummyController;

  protected:
    bool m_connected;
};

class ControllerGoingInCircles : public Controller {
  public:
    ControllerGoingInCircles(double da);
    virtual void onUpdate( const Time& elapsedTime) override;

  private:
    double m_da;
};

}  // namespace VVipers
#endif