#include <SFML/Window/Mouse.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/GameWindow.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

GameWindow::GameWindow()
    : sf::RenderWindow(sf::VideoMode(800, 600), "VoraciousVipers") {
    setMouseCursorGrabbed(true);
    setMouseCursorVisible(false);
    sf::Mouse::setPosition( sf::Vector2i(getSize().x / 2, getSize().y/2), *this );
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
            case sf::Event::MouseMoved: {
                const Vec2 screenHalfSize = getSize() / 2;
                const Vec2 relativeMouseMove =
                    (Vec2(event.mouseMove.x, event.mouseMove.y) -
                     screenHalfSize) /
                    screenHalfSize;
                MouseMoveEvent mouseMoveEvent(relativeMouseMove);
                notify(&mouseMoveEvent);
                sf::Mouse::setPosition(sf::Vector2i(getSize().x / 2, getSize().y/2), *this);
                tagInfo("move");
                break;
            }
        }
    }
}

}  // namespace VVipers