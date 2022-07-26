#include <vvipers/Food.hpp>

namespace VVipers {

const double Food::nominalFoodSize(40);

void Food::decay(Time elapsedTime) {
    static Time decayTime;
    if( m_startOfDecay ){
        m_startOfDecay = false;
        decayTime = seconds(0);
    }
    decayTime += elapsedTime;

    const double twirl = 5*360; // deg / s
    const Time timeOfDeath = seconds(1);

    m_body->convexShape.rotate(twirl * toSeconds(elapsedTime));
    m_body->convexShape.scale( (timeOfDeath - decayTime)/timeOfDeath * Vec2(1,1) );
    m_body->updateBodyPart();
    if (decayTime >= timeOfDeath)
        state(Dead);
}

void Food::stateChanged( ObjectState from, ObjectState into ){
    if( from == Alive && into == Dying)
        m_startOfDecay = true;
}

void Food::update(Time elapsedTime) {
    if (state() == Dead) {
        DestroyEvent event(this);
        notify(&event);
        return;
    }
    if (state() == Dying)
        decay(elapsedTime);
    else {
        m_body->convexShape.rotate(2*nominalFoodSize/m_size);
    }
}



}  // namespace VVipers
