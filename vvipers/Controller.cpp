#include <vvipers/Controller.hpp>
#include <vvipers/debug.hpp>
#include <SFML/Window/Mouse.hpp>

namespace VVipers {

void Controller::onNotify(const GameEvent* event) {
    if (event->type() != GameEvent::EventType::Update)
        throw std::runtime_error("Wrong event type sent to Controller.");
    update(static_cast<const UpdateEvent*>(event)->elapsedTime);
}

void KeyboardController::update(const Time& elapsedTime) {
    SteeringEvent event(this);
    // Only send event if something has changed since last time
    bool send_event = false;
    bool keyPressed;

    static bool lastLeft = false;
    keyPressed = sf::Keyboard::isKeyPressed(m_keys.left);
    if (keyPressed) {
        event.turn -= 1.0; // Hard turn left
        if (!lastLeft)
            send_event = true;
    } else if (lastLeft) {
        send_event = true;
    }
    lastLeft = keyPressed;

    static bool lastRight = false;
    keyPressed = sf::Keyboard::isKeyPressed(m_keys.right);
    if (keyPressed) {
        event.turn += 1.; // Hard turn right
        if (!lastRight)
            send_event = true;
    } else if (lastRight) {
        send_event = true;
    }
    lastRight = keyPressed;

    static bool lastBoost = false;
    keyPressed = sf::Keyboard::isKeyPressed(m_keys.boost);
    if (keyPressed) {
        event.boost = true;
        if (!lastBoost)
            send_event = true;
    } else if (lastBoost) {
        send_event = true;
    }
    lastBoost = keyPressed;

    if (send_event)
        notify(&event);
}

void MouseController::update( const Time& elapsedTime ){
    const sf::Vector2i windowHalfSize(m_window->getSize().x/2, m_window->getSize().y/2);
    const double angularSpeed = 180;
    SteeringEvent event(this);
    event.turn = angularSpeed * (sf::Mouse::getPosition(*m_window).x - windowHalfSize.x) / windowHalfSize.x;
    event.boost = sf::Mouse::isButtonPressed( sf::Mouse::Left );
    notify(&event);
    // This might cause problems if anything else uses the mouse
    sf::Mouse::setPosition(windowHalfSize, *m_window);
}

void ControllerGoingInCircles::update(const Time& elapsedTime) {
    SteeringEvent event(this);
    event.turn = 0.5;
    event.boost = true;
    notify(&event);
}

}  // namespace VVipers