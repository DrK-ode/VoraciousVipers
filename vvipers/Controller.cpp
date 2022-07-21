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
    if (sf::Keyboard::isKeyPressed(m_left)) {
        deltaAngle -= angularSpeed * toSeconds(elapsedTime);
    }
    if (sf::Keyboard::isKeyPressed(m_right)) {
        deltaAngle += angularSpeed * toSeconds(elapsedTime);
    }
    if (deltaAngle != 0) {
        SteeringEvent event(this, deltaAngle);
        notify(&event);
    }
}

void ControllerGoingInCircles::update(const Time& elapsedTime) {
    SteeringEvent event(this, m_da * toSeconds(elapsedTime));
    notify(&event);
}

}  // namespace VVipers