#include <vvipers/Scenes/Collision2/ColliderManager.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

std::unique_ptr<CollisionResult> ColliderManager::checkForCollisions() const {
    auto combinedResult = std::make_unique<CollisionResult>();
    auto nextResult = &combinedResult;
    for (auto coll1 = m_colliders.begin(); coll1 != m_colliders.end();
         ++coll1) {
        for (auto coll2 = coll1; coll2 != m_colliders.end(); ++coll2) {
            auto result = (*coll1)->collision(*(*coll2));
            if (result) {
                *nextResult = std::move(result);
                while (nextResult)
                    nextResult = &(*nextResult)->nextCollision;
            }
        }
    }
    return combinedResult;
}

std::unique_ptr<CollisionResult> ColliderManager::checkForCollisions(
    const Collider* c) const {
    auto combinedResult = std::make_unique<CollisionResult>();
    auto nextResult = &combinedResult;
    for (auto& collider : m_colliders) {
        auto result = collider->collision(*c);
        if (result) {
            *nextResult = std::move(result);
            while (nextResult)
                nextResult = &(*nextResult)->nextCollision;
        }
    }
    return combinedResult;
}

}  // namespace VVipers