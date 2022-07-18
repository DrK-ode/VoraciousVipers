#include <vvipers/CollisionDetector.hpp>
#include <vvipers/Collidable.hpp>

namespace VVipers {

void CollisionDetector::checkForCollisions() const {
    // Collect all the bodies from the collidables
    std::vector<const CollisionBody*> bodies;
    for (auto c : m_collidables) {
        for (auto& b : c->collisionBodies()) {
            bodies.push_back(b);
        }
    }

    auto b1 = bodies.cbegin();
    while (b1 != bodies.cend()) {
        auto b2 = b1;
        ++b2;
        while (b2 != bodies.cend()) {
            const CollisionBody* A = *b1;
            const CollisionBody* B = *b2;
            const auto& partCollisions = CollisionBody::collision(*A, *B);
            for (auto partCollision : partCollisions) {
                CollisionEvent event(A, partCollision.first, B,
                                     partCollision.second);
                notify(&event);
            }
            ++b2;
        }
        ++b1;
    }
}

}  // namespace VVipers