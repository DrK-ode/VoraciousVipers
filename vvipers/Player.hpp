#ifndef VVIPERS_PLAYER_HPP
#define VVIPERS_PLAYER_HPP

#include <string>
#include <vvipers/Score.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

class Controller;
class Viper;

class Player {
  public:
    Player(const std::string& name, Controller* c, Viper* v);
    std::string name() const { return m_name; }
    Viper* viper() const { return m_viper; }
    void viper(Viper* v) { m_viper = v; }
    const Controller* controller() const { return m_controller; }
    void controller(Controller* c) { m_controller = c; }

  private:
    std::string m_name;
    Score m_score;
    Viper* m_viper;
    Controller* m_controller;
};

}  // namespace VVipers
#endif