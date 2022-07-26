#include <vvipers/CollisionDetector.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

std::vector<Colliders> CollisionDetector::checkForCollisions() const {
    /** A collidable has several CollisionBodies that might collide with each
     * other. A Collidable can collide with itself but a CollisionBody cannot.
     * **/

    std::vector<Colliders> collisions;
    for (auto coll1 = m_collidables.begin(); coll1 != m_collidables.end();
         ++coll1)
        for (auto coll2 = coll1; coll2 != m_collidables.end(); ++coll2) {
            for (auto colliders : Collidable::collision(*coll1, *coll2)) {
                collisions.push_back(colliders);
            }
        }
    return collisions;
}

// Checks against collisions for a specific collidable
std::vector<Colliders> CollisionDetector::checkForCollisions(
    const Collidable* c) const {
    std::vector<Colliders> collisions;
    for (auto coll2 = m_collidables.begin(); coll2 != m_collidables.end();
         ++coll2) {
        for (auto colliders : Collidable::collision(c, *coll2)) {
            collisions.push_back(colliders);
        }
    }
    return collisions;
}

}  // namespace VVipers