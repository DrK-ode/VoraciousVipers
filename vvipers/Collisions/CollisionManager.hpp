#ifndef VVIPERS_COLLISION_COLLIDERMANAGER_HPP
#define VVIPERS_COLLISION_COLLIDERMANAGER_HPP

#include <set>
#include <vvipers/GameElements/GameObject.hpp>

#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/Utilities/Shape.hpp"

namespace VVipers {

struct CollisionItem {
    const CollidingBody* body;
    size_t index;
    bool operator<(const CollisionItem& other) const {
        if (body != other.body)
            return body < other.body;
        return index < other.index;
    }
};

using CollisionPair = std::pair<CollisionItem, CollisionItem>;

class CollisionManager {
  public:
    CollisionManager(size_t population_limit, double size_limit)
        : _population_limit(population_limit), _size_limit(size_limit) {}
    std::set<CollisionPair> check_for_collisions(
        const BoundingBox& starting_area) const;
    bool is_circle_occupied(const Vec2& center, double radius);
    bool is_rectangle_occupied(const Vec2& center, double width, double height,
                               double angle = 0.);
    void deregister_colliding_body(const CollidingBody* collider) {
        // If present remove it
        _colliding_bodies.erase(collider);
    }
    void register_colliding_body(const CollidingBody* collider) {
        _colliding_bodies.insert(collider);
    }

  private:
    bool is_occupied(const Shape&);
    std::set<const CollidingBody*> _colliding_bodies;

    size_t _population_limit;
    double _size_limit;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISION_COLLIDERMANAGER_HPP
