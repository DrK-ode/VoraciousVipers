#include <vvipers/Controller.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/Player.hpp>

namespace VVipers {

Player::Player( const std::string& name, Controller* c, Viper* v)
    : m_name(name), m_controller(c), m_viper(v) {}

}  // namespace VVipers