#ifndef VVIPERS_COLLISIONDETECTOR_HPP
#define VVIPERS_COLLISIONDETECTOR_HPP

#include <set>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/Observer.hpp>

namespace VVipers{

class Collidable;

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

}

#endif // VVIPERS_COLLISIONDETECTOR_HPP
