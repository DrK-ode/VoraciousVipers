#include <vvipers/GameEvent.hpp>
#include <vvipers/GameWindow.hpp>

namespace VVipers {

GameWindow::GameWindow()
    : sf::RenderWindow(sf::VideoMode(800, 600), "VoraciousVipers") {
        setMouseCursorGrabbed(true);
        setMouseCursorVisible(false);
        sf::Mouse::setPosition( {400,300} );
    }

GameWindow::~GameWindow() {}

void GameWindow::processEvents() {
    sf::Event event;
    while (pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
            case sf::Event::Resized: {
                WindowEvent wEvent(event.type);
                notify(&wEvent);
                break;
            }
            case sf::Event::KeyPressed: {
                KeyboardEvent keyEvent(event.key);
                notify(&keyEvent);
                break;
            }
        }
    }
}

}  // namespace VVipers