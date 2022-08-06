#include <vvipers/Scenes/Collision2/ColliderManager.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

CollisionVector ColliderManager::checkForCollisions() const {
    CollisionVector combinedResult;
    for (auto coll1 = m_colliders.begin(); coll1 != m_colliders.end();
         ++coll1) {
        for (auto coll2 = coll1; coll2 != m_colliders.end(); ++coll2) {
            auto result = Collider::collision(**coll1, **coll2);
            combinedResult.insert(combinedResult.end(), result.begin(),
                              result.end());
        }
    }
    return combinedResult;
}

CollisionVector ColliderManager::checkForCollisions(const Collider& c) const {
    CollisionVector combinedResult;
    for (auto& collider : m_colliders) {
        auto result = Collider::collision(*collider, c);
        combinedResult.insert(combinedResult.end(), result.begin(),
                              result.end());
    }
    return combinedResult;
}

}  // namespace VVipers