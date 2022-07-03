#include <vvipers/Controller.hpp>

ControllerGoingInCircles::ControllerGoingInCircles(float da) : m_da(da){}

void ControllerGoingInCircles::onTick(Viper& viper){
    viper.setAngle( viper.getAngle() + m_da );
    viper.growSegment(0.001);
}