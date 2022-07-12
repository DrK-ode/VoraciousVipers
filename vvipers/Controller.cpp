#include <vvipers/Controller.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

ControllerGoingInCircles::ControllerGoingInCircles(double da) : m_da(da) {}

void ControllerGoingInCircles::onUpdate(const Time& elapsedTime) {
    notify( SteeringEvent( m_da * toSeconds(elapsedTime) ) );
}

}  // namespace VVipers