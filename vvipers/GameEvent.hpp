#ifndef VVIPERS_GAMEEVENT_HPP
#define VVIPERS_GAMEEVENT_HPP

#include <string>
#include <vvipers/Collidable.hpp>

namespace VVipers {

class GameEvent {
  public:
    virtual GameEvent* clone() const = 0;
    std::string type() const { return m_type; }
    virtual ~GameEvent(){};

  protected:
    GameEvent(std::string type) : m_type(type) {}

  private:
    std::string m_type;
};

class CollisionEvent : public GameEvent {
  public:
    CollisionEvent(const Collidable* cA, const CollidablePartInterface* pA,
                   const Collidable* cB, const CollidablePartInterface* pB)
        : GameEvent("Collision"), A(cA), B(cB), A_part(pA), B_part(pB) {}
    GameEvent* clone() const override { return new CollisionEvent(*this); }
    const Collidable* const A;
    const Collidable* const B;
    const CollidablePartInterface* const A_part;
    const CollidablePartInterface* const B_part;
};

class SteeringEvent : public GameEvent {
  public:
    SteeringEvent(double dA) : GameEvent("Steering"), deltaAngle(dA) {}
    GameEvent* clone() const override { return new SteeringEvent(*this); }
    const double deltaAngle;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEEVENT_HPP
