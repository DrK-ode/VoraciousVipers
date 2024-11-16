#include <vvipers/UIElements/Controller.hpp>
#include <vvipers/GameElements/Player.hpp>
#include <vvipers/GameElements/Viper.hpp>

namespace VVipers {

Player::Player(const std::string& name, std::unique_ptr<Controller> c, Viper* v)
    : m_name(name),
      m_primaryColor(sf::Color::Green),
      m_secondaryColor(sf::Color::Red),
      m_controller(std::move(c)),
      m_score(0),
      m_viper(v) {}

void Player::setColors(sf::Color c1, sf::Color c2) {
    m_primaryColor = c1;
    m_secondaryColor = c2;
    if (m_viper)
        m_viper->set_colors(c1,c2);
    ObjectModifiedEvent event(this);
    notify(&event);
}

}  // namespace VVipers