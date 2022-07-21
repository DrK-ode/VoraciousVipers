#include <vvipers/Controller.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

void Controller::onNotify(const GameEvent* event) {
    if (event->type() != GameEvent::EventType::Update)
        throw std::runtime_error("Wrong event type sent to Controller.");
    update(static_cast<const UpdateEvent*>(event)->elapsedTime);
}

void KeyboardController::update(const Time& elapsedTime) {
    const double angularSpeed = 180;
    double deltaAngle = 0;
    SteeringEvent event(this);
    bool send_event = false;
    if (sf::Keyboard::isKeyPressed(m_keys.left)) {
        event.deltaAngle -= angularSpeed * toSeconds(elapsedTime);
        send_event = true;
    }
    if (sf::Keyboard::isKeyPressed(m_keys.right)) {
        event.deltaAngle += angularSpeed * toSeconds(elapsedTime);
        send_event = true;
    }
    if (sf::Keyboard::isKeyPressed(m_keys.boost)) {
        event.boost = true;
        send_event = true;
    }
    if (send_event)
        notify(&event);
}

void ControllerGoingInCircles::update(const Time& elapsedTime) {
    SteeringEvent event(this);
    event.deltaAngle = m_da * toSeconds(elapsedTime);
    notify(&event);
}

}  // namespace VVipers