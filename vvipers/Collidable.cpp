#include <limits>
#include <vvipers/Bodypart.hpp>
#include <vvipers/Collidable.hpp>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

std::vector<Colliders> Collidable::collision(const Collidable* coll1,
                                             const Collidable* coll2) {
    std::vector<Colliders> colliders;
    // Rough check before doing it properly
    if( !coll1->rectangularBounds().intersects(coll2->rectangularBounds()))
        return colliders;
        
    const auto& bodies1 = coll1->collisionBodies();
    const auto& bodies2 = coll2->collisionBodies();
    for (auto body1 = bodies1.cbegin(); body1 != bodies1.cend(); ++body1) {
        /** Avoid double checking (and counting) if were looking at
         * internal collisions within a Collidable **/
        auto body2start = (coll1 == coll2) ? body1 + 1 : bodies2.cbegin();
        auto body2end = (coll1 == coll2) ? bodies1.cend() : bodies2.cend();
        for (auto body2 = body2start; body2 != body2end; ++body2) {
            const auto bodypartCollisions =
                CollisionBody::collision(*body1, *body2);
            for (auto collision : bodypartCollisions) {
                colliders.emplace_back(
                    CollisionTriplet(coll1, *body1, collision.first),
                    CollisionTriplet(coll2, *body2, collision.second));
            }
        }
    }
    return colliders;
}

sf::Rect<double> Collidable::rectangularBounds() const {
    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();

    for (auto body : collisionBodies()) {
        auto bounds = body->rectangularBounds();
        xmin = std::min(xmin, bounds.left);
        xmax = std::max(xmax, bounds.left + bounds.width);
        ymin = std::min(ymin, bounds.top);
        ymax = std::max(ymax, bounds.top + bounds.height);
    }
    return sf::Rect<double>(xmin, ymin, xmax - xmin, ymax - ymin);
}

}  // namespace VVipers