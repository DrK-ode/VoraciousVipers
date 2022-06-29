#include "GameManager.hpp"

GameManager::GameManager() {
    m_window = new GameWindow;
    m_window->setFramerateLimit(60);
    m_game = new Game;
}

GameManager::~GameManager() {
    delete m_window;
    delete m_game;
}

void GameManager::startGame() {
    sf::Clock clock;
    while (m_window->isOpen()) {
        m_game->tick( clock.restart());

        sf::Event event;
        while (m_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) m_window->close();
        }

        m_window->clear(sf::Color::Black);
        m_window->draw(*m_game);
        m_window->display();
    }
}