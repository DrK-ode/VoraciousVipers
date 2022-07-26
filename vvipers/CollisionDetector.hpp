#ifndef VVIPERS_COLLISIONDETECTOR_HPP
#define VVIPERS_COLLISIONDETECTOR_HPP

#include <set>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/Collidable.hpp>

namespace VVipers{

class CollisionDetector  {
  public:
    std::vector<Colliders> checkForCollisions() const;
    std::vector<Colliders> checkForCollisions(const Collidable* c) const;
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
