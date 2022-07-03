#include <vvipers/Player.hpp>

Player::Player(std::string name) : m_name(name), m_controller(new Controller) {}

Player::~Player(){
    delete m_controller;
}