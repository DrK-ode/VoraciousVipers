#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionDetector.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

void CollisionDetector::checkForCollisions() const {
    /** A collidable has several CollisionBodies that might collide with each
     * other. A Collidable can collide with itself but a CollisionBody cannot.
     * **/

    for (auto coll1 = m_collidables.begin(); coll1 != m_collidables.end();
         ++coll1)
        for (auto coll2 = coll1; coll2 != m_collidables.end(); ++coll2) {
            for (auto colliders : Collidable::collision(*coll1, *coll2)) {
                CollisionEvent event(colliders);
                notify(&event);
            }
        }
}

}  // namespace VVipers