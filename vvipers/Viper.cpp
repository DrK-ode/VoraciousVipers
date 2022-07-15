#include <vvipers/Viper.hpp>

namespace VVipers {

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_viperGraph, states);
}

void Viper::onNotify(const GameEvent* event) {
    if( event->type() != GameEvent::EventType::Update )
        throw std::runtime_error("Wrong event type sent to Viper.");
    update( static_cast<const UpdateEvent*>(event)->elapsedTime );
}


void Viper::setup(const Vec2& from, double angle, const Time& length) {
    m_viperPhys.setup(from, angle, length);
}

void Viper::update(const Time& elapsedTime) {
    m_viperPhys.update(elapsedTime);
    m_viperGraph.update(m_viperPhys);
}
}  // namespace VVipers