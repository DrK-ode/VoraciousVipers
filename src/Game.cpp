#include "Game.hpp"

#include "debug.hpp"

Game::Game() {
    m_viper = new Viper;
    m_viper->setupStart({400.f, 100.f}, 0.f , 2);
}

Game::~Game() { delete m_viper; }

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(*m_viper, states);
}

void Game::tick(sf::Time elapsedTime) {
    m_viper->tick( elapsedTime );
}