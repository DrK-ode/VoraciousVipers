#include <vvipers/Viper.hpp>

namespace VVipers {

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_viperGraph, states);
}

void Viper::setup(const Vec2& from, float angle, const Time& length) {
    m_viperPhys.setup(from, angle, length);
}
void Viper::update(const Time& elapsedTime) {
    m_viperPhys.update(elapsedTime);
    m_viperGraph.update(elapsedTime, m_viperPhys);
}
}  // namespace VVipers