#include <vvipers/Scenes/UIElements/Controller.hpp>
#include <vvipers/Scenes/GameElements/Player.hpp>
#include <vvipers/Scenes/GameElements/Viper.hpp>

namespace VVipers {

Player::Player(const std::string& name, std::shared_ptr<Controller> c, std::shared_ptr<Viper> v)
    : m_name(name),
      m_color(sf::Color::Green),
      m_controller(c),
      m_score(0),
      m_viper(v) {}

void Player::color(sf::Color c) {
    m_color = c;
    if (m_viper)
        m_viper->color(c);
    ObjectModifiedEvent event(this);
    notify(&event);
}

}  // namespace VVipers