#include <vvipers/Controller.hpp>

namespace VVipers {

ControllerGoingInCircles::ControllerGoingInCircles(float da) : m_da(da) {}

void ControllerGoingInCircles::onTick(const sf::Time& elapsedTime, Viper& viper) {
    viper.setAngle(viper.getAngle() + m_da);
    viper.growth( elapsedTime * 0.5f );
}

}  // namespace VVipers