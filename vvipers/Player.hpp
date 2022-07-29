#ifndef VVIPERS_PLAYER_HPP
#define VVIPERS_PLAYER_HPP

#include <SFML/Graphics/Color.hpp>
#include <string>
#include <vvipers/debug.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/GameObject.hpp>

namespace VVipers {

class Controller;
class Viper;

typedef uint64_t score_t;
typedef uint64_t level_t;

class Player : public GameObject, public Observable {
  public:
    Player(const std::string& name, Controller* c, Viper* v);

    static level_t calculateLevel(score_t score) { return score / 1000 + 1; }
    static score_t calculateLevelLimit(level_t level) {
        return (level - 1) * 1000;
    }

    sf::Color color() const { return m_color; }
    void color(sf::Color c);
    const Controller* controller() const { return m_controller; }
    void controller(Controller* c) { m_controller = c; }
    std::string name() const { return m_name; }
    score_t score() const { return m_score; };
    void score(score_t score) { m_score += score; };
    Viper* viper() const { return m_viper; }
    void viper(Viper* v) { m_viper = v; }

  private:
    std::string m_name;
    sf::Color m_color;
    Controller* m_controller;
    score_t m_score;
    Viper* m_viper;
};

}  // namespace VVipers
#endif