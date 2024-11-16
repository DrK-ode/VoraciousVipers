#include <vector>
#include <vvipers/Collisions/CollisionManager.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

std::vector<CollisionManager::CollisionEntity>
CollisionManager::collect_collision_items() {
    std::vector<CollisionEntity> body_parts;
    for (const CollidingBody* body : _colliding_bodies) {
        for (size_t i = 0; i < body->number_of_body_parts(); ++i) {
            body_parts.emplace_back(body, i);
        }
    }
    return body_parts;
}

CollisionVector CollisionManager::check_for_collisions() {
    auto collision_items = collect_collision_items();

    CollisionVector all_collisions;
    for (auto first_item_iter = collision_items.begin();
         first_item_iter != collision_items.end(); ++first_item_iter) {
        for (auto second_item_iter = first_item_iter + 1;
             second_item_iter != collision_items.end(); ++second_item_iter) {
            if (first_item_iter->shape->overlap(*second_item_iter->shape)) {
                all_collisions.emplace_back(
                    CollisionItem(first_item_iter->body,
                                    first_item_iter->index),
                    CollisionItem(second_item_iter->body,
                                    second_item_iter->index));
            }
        }
    }
    for( auto& c : all_collisions){
        tagDebug(c.first.body->name(), "[", c.first.index, "] collided with ", c.second.body->name(), "[", c.second.index, "].");
    }
    return all_collisions;
}

bool CollisionManager::is_occupied(const Shape& test_object) {
    auto collision_items = collect_collision_items();
    for (auto segment_iter = collision_items.begin();
         segment_iter != collision_items.end(); ++segment_iter) {
        if (test_object.overlap(*segment_iter->shape)) {
            return true;
        }
    }
    return false;
}

bool CollisionManager::is_circle_occupied(const Vec2& center, double radius) {
    Circle test_object = Circle(center, radius);
    return is_occupied(test_object);
}

bool CollisionManager::is_rectangle_occupied(const Vec2& center, double width,
                                             double height, double angle) {
    std::vector<Vec2> corners;
    double half_w = 0.5 * width;
    double half_h = 0.5 * height;
    corners.emplace_back(center.x - half_w, center.y + half_h);
    corners.emplace_back(center.x + half_w, center.y + half_h);
    corners.emplace_back(center.x + half_w, center.y - half_h);
    corners.emplace_back(center.x - half_w, center.y - half_h);
    Polygon test_object = Polygon(center, corners);
    test_object.rotate(angle);
    return is_occupied(test_object);
}

}  // namespace VVipers