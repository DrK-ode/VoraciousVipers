#include <tuple>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

std::vector<Colliders> Collidable::collision(const Collidable* coll1,
                                             const Collidable* coll2) {
    std::vector<Colliders> colliders;
    const auto& bodies1 = coll1->collisionBodies();
    /* Make sure to use the same vector if the Collidables are the same since
     * its a copy that is returned. Important for iterator logic further down.
     WHY DOESN'T THIS WORK?
     const auto& bodies2 = (coll1 == coll2) ? bodies1 : coll2->collisionBodies();
     */
    const auto& bodies2 = coll2->collisionBodies();
    int count1 = 0;
    for (auto body1 = bodies1.cbegin(); body1 != bodies1.cend(); ++body1) {
        /** Avoid double checking (and counting) if were looking at
         * internal collisions within a Collidable **/
        int count2 = 0;
        auto body2start = (coll1 == coll2) ? body1 + 1 : bodies2.cbegin();
        auto body2end = (coll1 == coll2) ? bodies1.cend() : bodies2.cend();
        for (auto body2 = body2start; body2 != body2end; ++body2) {
            const auto bodypartCollisions =
                CollisionBody::collision(*body1, *body2);
            for (auto collision : bodypartCollisions) {
                colliders.emplace_back(
                    CollisionTuple(coll1, *body1, collision.first),
                    CollisionTuple(coll2, *body2, collision.second));
            }
        }
    }
    return colliders;
}

}  // namespace VVipers