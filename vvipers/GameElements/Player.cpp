#include <vvipers/GameElements/Player.hpp>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/UIElements/Controller.hpp>

namespace VVipers {

Player::Player(const std::string& name, std::unique_ptr<Controller> controller,
               std::unique_ptr<Viper> viper)
    : _name(name),
      _primary_color(sf::Color::Green),
      _secondary_color(sf::Color::Red),
      _controller(std::move(controller)),
      _score(0),
      _viper(std::move(viper)) {}

void Player::set_colors(sf::Color c1, sf::Color c2) {
    _primary_color = c1;
    _secondary_color = c2;
    if (_viper)
        _viper->set_colors(c1, c2);
    notify(ObjectModifiedEvent(this));
}

}  // namespace VVipers