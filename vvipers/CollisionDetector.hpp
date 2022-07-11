#ifndef VVIPERS_COLLISIONDETECTOR_HPP
#define VVIPERS_COLLISIONDETECTOR_HPP

#include <algorithm>
#include <set>
#include <vvipers/Collidable.hpp>
#include <vvipers/Observer.hpp>

namespace VVipers {

class CollisionDetector : public Observable {
  public:
    void checkForCollisions() const;
    void deRegisterCollidable(Collidable* c) {
        // If present remove it
        m_collidables.erase(c);
    }
    void registerCollidable(Collidable* c) { m_collidables.insert(c); }

  private:
    std::set<const Collidable*> m_collidables;
};

}  // namespace VVipers

#endif // VVIPERS_COLLISIONDETECTOR_HPP
