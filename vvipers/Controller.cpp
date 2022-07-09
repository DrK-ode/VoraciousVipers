#include <vvipers/Controller.hpp>

namespace VVipers {

ControllerGoingInCircles::ControllerGoingInCircles(double da) : m_da(da) {}

void ControllerGoingInCircles::onUpdate(const Time& elapsedTime, Viper& viper) {
    ViperPhysics& viperPhys = viper.getPhysicalViper();
    viperPhys.setAngle(viperPhys.angle() + m_da * toSeconds(elapsedTime) );
    //viperPhys.growth(elapsedTime / 2);
}

}  // namespace VVipers