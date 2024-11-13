#ifndef VVIPERS_COLLISION_COLLIDERMANAGER_HPP
#define VVIPERS_COLLISION_COLLIDERMANAGER_HPP

#include <memory>
#include <set>
#include <vvipers/GameElements/GameObject.hpp>

#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/Collisions/CollidingSegment.hpp"

namespace VVipers {

using CollisionPointer = std::shared_ptr<const CollidingSegment>;
using CollisionPair = std::pair<CollisionPointer, CollisionPointer>;
using CollisionVector = std::vector<CollisionPair>;

class CollisionManager {
  public:
    CollisionVector check_for_collisions();
    bool is_circle_occupied(const Vec2& center, double radius);
    bool is_rectangle_occupied(const Vec2& center, double width, double height, double angle = 0.);
    void deregister_colliding_body(const CollidingBody& collider) {
        // If present remove it
        _colliding_bodies.erase(&collider);
    }
    void register_colliding_body(const CollidingBody& collider) {
        _colliding_bodies.insert(&collider);
    }

  private:
    bool is_occupied(const CollidingSegment&);
    void update_colliding_segments();
    std::set<const CollidingBody*> _colliding_bodies;
    std::vector<CollisionPointer> _colliding_segments;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISION_COLLIDERMANAGER_HPP
