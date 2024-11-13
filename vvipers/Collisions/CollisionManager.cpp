#include <iterator>
#include <memory>
#include <vector>
#include <vvipers/Collisions/CollisionManager.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/Collisions/CollidingSegment.hpp"

namespace VVipers {

void CollisionManager::update_colliding_segments() {
    _colliding_segments.clear();
    for (auto body : _colliding_bodies) {
        auto segments = body->colliding_segments();
        _colliding_segments.insert(_colliding_segments.end(),
                                   std::make_move_iterator(segments.begin()),
                                   std::make_move_iterator(segments.end()));
    }
}

CollisionVector CollisionManager::check_for_collisions() {
    update_colliding_segments();
    CollisionVector all_collisions;
    for (auto first_segment_iter = _colliding_segments.begin();
         first_segment_iter != _colliding_segments.end();
         ++first_segment_iter) {
        for (auto second_segment_iter = first_segment_iter;
             second_segment_iter != _colliding_segments.end();
             ++second_segment_iter) {
            if (first_segment_iter->get()->collision(
                    *second_segment_iter->get())) {
                all_collisions.emplace_back(*first_segment_iter,
                                            *second_segment_iter);
            }
        }
    }
    return all_collisions;
}

bool CollisionManager::is_occupied(const CollidingSegment& test_object) {
    update_colliding_segments();
    std::vector<CollisionPointer> all_collisions;
    for (auto segment_iter = _colliding_segments.begin();
         segment_iter != _colliding_segments.end(); ++segment_iter) {
        if (test_object.collision(*segment_iter->get())) {
            return true;
        }
    }
    return false;
}

bool CollisionManager::is_circle_occupied(const Vec2& center, double radius) {
    CollidingCircle test_object = CollidingCircle(center, radius, nullptr);
    return is_occupied(test_object);
}

bool CollisionManager::is_rectangle_occupied(const Vec2& center, double width,
                                   double height, double angle) {
    std::vector<Vec2> corners;
    double half_w = 0.5 * width;
    double half_h = 0.5 * height;
    const double cos = std::cos(angle);
    const double sin = std::sin(angle);
    corners.emplace_back(center.x - half_w * cos - half_h * sin,
                         center.y + half_h * cos - half_w * sin);
    corners.emplace_back(center.x + half_w * cos - half_h * sin,
                         center.y + half_h * cos + half_w * sin);
    corners.emplace_back(center.x + half_w * cos + half_h * sin,
                         center.y - half_h * cos + half_w * sin);
    corners.emplace_back(center.x - half_w * cos + half_h * sin,
                         center.y - half_h * cos - half_w * sin);
    CollidingPolygon test_object = CollidingPolygon(corners, nullptr);
    return is_occupied(test_object);
}

}  // namespace VVipers