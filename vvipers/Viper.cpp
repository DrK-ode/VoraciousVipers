#include <vvipers/Viper.hpp>

namespace VVipers {

Viper::~Viper() { setController(nullptr); }

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_viperGraph, states);
}

void Viper::onNotify(const GameEvent& event) {
    try {
        const SteeringEvent& steering =
            dynamic_cast<const SteeringEvent&>(event);
        m_viperPhys.setAngle(m_viperPhys.angle() + steering.deltaAngle);
    } catch (std::bad_cast) {
    }  // Do nothing, it's just another type of event
}

void Viper::setController(Controller* ctrl) {
    if (m_controller)
        m_controller->removeObserver(this);
    if (m_controller = ctrl)
        ctrl->addObserver(this);
}

void Viper::setup(const Vec2& from, double angle, const Time& length) {
    m_viperPhys.setup(from, angle, length);
}

void Viper::update(const Time& elapsedTime) {
    m_viperPhys.update(elapsedTime);
    m_viperGraph.update(m_viperPhys);
}
}  // namespace VVipers