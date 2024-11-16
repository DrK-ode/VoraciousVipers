#ifndef VVIPERS_COLLISION_COLLIDERMANAGER_HPP
#define VVIPERS_COLLISION_COLLIDERMANAGER_HPP

#include <memory>
#include <set>
#include <vvipers/GameElements/GameObject.hpp>

#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/Utilities/Shape.hpp"

namespace VVipers {

struct CollisionItem {
    const CollidingBody* body;
    size_t index;
};
using CollisionPair = std::pair<CollisionItem, CollisionItem>;
using CollisionVector = std::vector<CollisionPair>;

class CollisionManager {
  private:
    struct CollisionEntity {
        CollisionEntity(const CollidingBody* body, size_t body_part_index)
            : index(body_part_index),
              body(body),
              shape(body->body_part_shape(body_part_index)),
              bounding_box(shape->bounding_box()) {}
        const size_t index;
        const CollidingBody* body;
        const std::shared_ptr<const Shape> shape;
        const BoundingBox bounding_box;
    };

  public:
    CollisionVector check_for_collisions();
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
    std::vector<CollisionEntity> collect_collision_items();
    std::set<const CollidingBody*> _colliding_bodies;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISION_COLLIDERMANAGER_HPP
