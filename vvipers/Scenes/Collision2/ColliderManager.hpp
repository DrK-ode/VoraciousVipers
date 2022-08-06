#ifndef VVIPERS_SCENES_COLLISION2_COLLIDERMANAGER_HPP
#define VVIPERS_SCENES_COLLISION2_COLLIDERMANAGER_HPP

#include <set>
#include <vector>
#include <memory>
#include <vvipers/Scenes/Collision2/Collider.hpp>

namespace VVipers {

class ColliderManager {
  public:
    std::unique_ptr<CollisionResult> checkForCollisions() const;
    std::unique_ptr<CollisionResult> checkForCollisions(const Collider* c) const;
    void deRegisterCollider(Collider* collider) {
        // If present remove it
        m_colliders.erase(collider);
    }
    void registerCollider(const Collider* collider) {
        m_colliders.insert(collider);
    }

  private:
    std::set<const Collider*> m_colliders;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_COLLISION2_COLLIDERMANAGER_HPP
