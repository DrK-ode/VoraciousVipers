#pragma once

#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <string>
#include <vvipers/GameElements/Controller.hpp>
#include <vvipers/GameElements/GameObject.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

typedef uint64_t score_t;
typedef uint64_t level_t;

class Player : public GameObject, public Observable {
  public:
    Player(const std::string& name, std::unique_ptr<Controller>,
           std::unique_ptr<Viper>);

    static level_t calculate_level(score_t score) { return score / 1000 + 1; }
    static score_t calculate_level_limit(level_t level) {
        return (level - 1) * 1000;
    }

    sf::Color primary_color() const { return _primary_color; }
    sf::Color secondary_color() const { return _secondary_color; }
    void set_colors(sf::Color c1, sf::Color c2);
    const Controller* controller() const { return _controller.get(); }
    std::string name() const { return _name; }
    score_t score() const { return _score; };
    void score(score_t score) { _score += score; };
    Viper* viper() const { return _viper.get(); }
    // void viper(std::shared_ptr<Viper> v) { m_viper = v; }

  private:
    std::string _name;
    sf::Color _primary_color;
    sf::Color _secondary_color;
    std::unique_ptr<Controller> _controller;
    score_t _score;
    std::unique_ptr<Viper> _viper;
};

}  // namespace VVipers
