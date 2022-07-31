#ifndef VVIPERS_PLAYER_HPP
#define VVIPERS_PLAYER_HPP

#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <vvipers/Scenes/GameElements/Controller.hpp>
#include <vvipers/Scenes/GameElements/GameObject.hpp>
#include <vvipers/Scenes/GameElements/Observer.hpp>
#include <vvipers/Scenes/GameElements/Viper.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

typedef uint64_t score_t;
typedef uint64_t level_t;

class Player : public GameObject, public Observable {
  public:
    Player(const std::string& name, controller_ptr c, viper_ptr v);

    static level_t calculateLevel(score_t score) { return score / 1000 + 1; }
    static score_t calculateLevelLimit(level_t level) {
        return (level - 1) * 1000;
    }

    sf::Color color() const { return m_color; }
    void color(sf::Color c);
    const Controller* controller() const { return m_controller.get(); }
    void controller(controller_ptr c) { m_controller = c; }
    std::string name() const { return m_name; }
    score_t score() const { return m_score; };
    void score(score_t score) { m_score += score; };
    Viper* viper() const { return m_viper.get(); }
    void viper(viper_ptr v) { m_viper = v; }

  private:
    std::string m_name;
    sf::Color m_color;
    controller_ptr m_controller;
    score_t m_score;
    viper_ptr m_viper;
};

using player_ptr = std::shared_ptr<Player>;

}  // namespace VVipers
#endif