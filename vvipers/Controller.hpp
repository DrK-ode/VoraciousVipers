#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <vvipers/Viper.hpp>
#include <SFML/System/Time.hpp>

namespace VVipers {

class Controller {
  public:
    Controller() : m_connected(false) {}
    virtual void onUpdate(const sf::Time& elapsedTime, Viper&){};
    virtual void onKeyboard(Viper&){};
    virtual void onMouse(Viper&){};

    static const Controller dummyController;

  protected:
    bool m_connected;
};

class ControllerGoingInCircles : public Controller {
  public:
    ControllerGoingInCircles(float da);
    virtual void onUpdate( const sf::Time& elapsedTime, Viper&) override;

  private:
    float m_da;
};

}  // namespace VVipers
#endif