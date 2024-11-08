#ifndef VVIPERS_COLLISION_COLLIDERMANAGER_HPP
#define VVIPERS_COLLISION_COLLIDERMANAGER_HPP

#include <set>
#include <vvipers/Collision/Collider.hpp>
#include <vvipers/GameElements/GameObject.hpp>

namespace VVipers {

class ColliderManager {
  public:
    CollisionVector checkForCollisions() const;
    CollisionVector checkForCollisions(const Collider& c) const;
    void deRegisterCollider(const Collider& collider) {
        // If present remove it
        m_colliders.erase(&collider);
    }
    void registerCollider(const Collider& collider) {
        m_colliders.insert(&collider);
    }

  private:
    std::set<const Collider*> m_colliders;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISION_COLLIDERMANAGER_HPP
