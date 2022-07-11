#ifndef VVIPERS_GAMEEVENT_HPP
#define VVIPERS_GAMEEVENT_HPP

#include <string>
#include <vvipers/Collidable.hpp>

namespace VVipers {

class GameEvent {
  public:
    std::string type() const { return m_type; }

  protected:
    GameEvent(std::string type) : m_type(type) {}

  private:
    std::string m_type;
};

class CollisionEvent : public GameEvent {
  public:
    CollisionEvent(const Collidable* cA, const CollidablePart* pA,
                   const Collidable* cB, const CollidablePart* pB)
        : GameEvent("Collision"), A(cA), B(cB), A_part(pA), B_part(pB) {}
    const Collidable* const A;
    const Collidable* const B;
    const CollidablePart* const A_part;
    const CollidablePart* const B_part;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEEVENT_HPP
