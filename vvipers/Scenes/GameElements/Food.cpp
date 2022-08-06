#include <vvipers/Scenes/GameElements/Food.hpp>

namespace VVipers {

const double Food::nominalFoodRadius(20);

void Food::decay(Time elapsedTime) {
    static Time decayTime;
    if( m_startOfDecay ){
        m_startOfDecay = false;
        decayTime = seconds(0);
    }
    decayTime += elapsedTime;

    const double twirl = 5*360; // deg / s
    const Time timeOfDeath = seconds(1);

    rotate(twirl * toSeconds(elapsedTime));
    scale( (timeOfDeath - decayTime)/timeOfDeath * Vec2(1,1) );
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
        rotate(nominalFoodRadius/getRadius());
    }
}



}  // namespace VVipers
