#include <vvipers/VVipers.hpp>
#include <vvipers/Time.hpp>

namespace VVipers {

VVipers::VVipers() {
    m_window = new GameWindow;
    m_game = new Game;
}

VVipers::~VVipers() {
    delete m_window;
    delete m_game;
}

void VVipers::startGame() {
    sf::Clock clock;
    while (m_window->isOpen()) {
        if ( seconds(clock.getElapsedTime().asSeconds()) > seconds(0.01) ) // TODO: Need better time handling
            m_game->update( seconds(clock.restart().asSeconds() )); //TODO: fix this mess

        sf::Event event;
        while (m_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window->close();
        }

        m_window->clear(sf::Color::Black);
        m_window->draw(*m_game);
        m_window->display();
    }
}

}  // namespace VVipers