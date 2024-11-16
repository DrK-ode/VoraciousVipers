#ifndef VVIPERS_GAMEELEMENTS_PLAYER_HPP
#define VVIPERS_GAMEELEMENTS_PLAYER_HPP

#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <string>
#include <vvipers/UIElements/Controller.hpp>
#include <vvipers/GameElements/GameObject.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

typedef uint64_t score_t;
typedef uint64_t level_t;

class Player : public GameObject, public Observable {
  public:
    Player(const std::string& name, std::unique_ptr<Controller> c, Viper* v);

    static level_t calculateLevel(score_t score) { return score / 1000 + 1; }
    static score_t calculateLevelLimit(level_t level) {
        return (level - 1) * 1000;
    }

    sf::Color getPrimaryColor() const { return m_primaryColor; }
    sf::Color getSecondaryColor() const { return m_secondaryColor; }
    void setColors(sf::Color c1, sf::Color c2);
    const Controller* controller() const { return m_controller.get(); }
    //void controller(std::shared_ptr<Controller> c) { m_controller = c; }
    std::string name() const { return m_name; }
    score_t score() const { return m_score; };
    void score(score_t score) { m_score += score; };
    Viper* viper() const { return m_viper; }
    //void viper(std::shared_ptr<Viper> v) { m_viper = v; }

  private:
    std::string m_name;
    sf::Color m_primaryColor;
    sf::Color m_secondaryColor;
    std::unique_ptr<Controller> m_controller;
    score_t m_score;
    Viper* m_viper;
};

}  // namespace VVipers
#endif // VVIPERS_GAMEELEMENTS_PLAYER_HPP
