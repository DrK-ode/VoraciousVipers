#include <vvipers/Controller.hpp>
#include <vvipers/Player.hpp>
#include <vvipers/Viper.hpp>

namespace VVipers {

Player::Player(const std::string& name, Controller* c, Viper* v)
    : m_name(name),
      m_color(sf::Color::Green),
      m_controller(c),
      m_viper(v),
      m_score(0) {}

void Player::color(sf::Color c) {
    m_color = c;
    if (m_viper)
        m_viper->color(c);
}

}  // namespace VVipers