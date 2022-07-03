#include <vvipers/Player.hpp>

namespace VVipers {

Player::Player(std::string name) : m_name(name), m_controller(new Controller) {}

Player::~Player(){
    delete m_controller;
}

}