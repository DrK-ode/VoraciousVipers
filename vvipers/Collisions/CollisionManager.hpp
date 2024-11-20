#pragma once

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
    void deregister_colliding_body(const CollidingBody* collider) {
        // If present remove it
        _colliding_bodies.erase(collider);
    }
    bool is_occupied(const Shape&) const;
    void register_colliding_body(const CollidingBody* collider) {
        _colliding_bodies.insert(collider);
    }

  private:
    std::set<const CollidingBody*> _colliding_bodies;

    size_t _population_limit;
    double _size_limit;
};

}  // namespace VVipers
