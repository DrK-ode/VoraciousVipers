#include <vvipers/Controller.hpp>

namespace VVipers {

ControllerGoingInCircles::ControllerGoingInCircles(float da) : m_da(da) {}

void ControllerGoingInCircles::onUpdate(const Time& elapsedTime, Viper& viper) {
    ViperPhysics& viperPhys = viper.getPhysicalViper();
    viperPhys.setAngle(viperPhys.angle() + m_da);
    viperPhys.growth( elapsedTime * 0.5 );
}

}  // namespace VVipers