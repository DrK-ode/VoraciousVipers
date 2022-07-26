#ifndef VVIPERS_PLAYER_HPP
#define VVIPERS_PLAYER_HPP

#include <string>
#include <vvipers/Score.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

class Controller;
class Viper;

typedef uint64_t score_t;

class Player {
  public:
    Player(const std::string& name, Controller* c, Viper* v);

    const Controller* controller() const { return m_controller; }
    void controller(Controller* c) { m_controller = c; }
    std::string name() const { return m_name; }
    score_t score() const {return m_score;};
    void score(score_t score) {m_score += score;};
    Viper* viper() const { return m_viper; }
    void viper(Viper* v) { m_viper = v; }

  private:
    std::string m_name;
    score_t m_score;
    Viper* m_viper;
    Controller* m_controller;
};

}  // namespace VVipers
#endif