#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <vvipers/Viper.hpp>

class Controller {
  public:
    Controller() : m_connected(false){}
    virtual void onTick(Viper&){};
    virtual void onKeyboard(Viper&){};
    virtual void onMouse(Viper&){};

    static const Controller dummyController;

    protected:
    bool m_connected;
};

class ControllerGoingInCircles : public Controller {
  public:
    ControllerGoingInCircles(float da);
    virtual void onTick(Viper&) override;

  private:
    float m_da;
};

#endif