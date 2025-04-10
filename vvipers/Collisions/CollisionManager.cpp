#include <algorithm>
#include <memory>
#include <ranges>
#include <vector>

#include "vvipers/Collisions/CollidingBody.hpp"
#include <vvipers/Collisions/CollisionManager.hpp>
#include <vvipers/Utilities/debug.hpp>
#include "vvipers/Utilities/Shape.hpp"

namespace VVipers {

struct CollisionEntity {
  CollisionEntity(const CollidingBody* body, size_t body_part_index)
    : index(body_part_index),
      body(body),
      shape(body->segment_shape(body_part_index)),
      bounding_box(shape->bounding_box()) {}
  const size_t index;
  const CollidingBody* body;
  const std::shared_ptr<const Shape> shape;
  const BoundingBox bounding_box;
};

void collision_check(
  const std::vector<std::shared_ptr<const CollisionEntity>>& entities,
  std::set<std::pair<CollisionItem, CollisionItem>>& all_collisions) {
  for (const auto& [index, first_item] :
       entities | std::ranges::views::enumerate) {
    for (const auto& second_item : entities | std::views::drop(index + 1)) {
      if (first_item->shape->overlap(*second_item->shape)) {
        CollisionItem item1(first_item->body, first_item->index);
        CollisionItem item2(second_item->body, second_item->index);
        all_collisions.emplace(std::move(item1), std::move(item2));
      }
    }
  }
}

void collision_quad_tree(
  const std::vector<std::shared_ptr<const CollisionEntity>>& entities,
  const BoundingBox& area, size_t size_limit, double population_limit,
  std::set<std::pair<CollisionItem, CollisionItem>>& all_collisions) {
  double x_mid = 0.5 * (area.x_max + area.x_min);
  double y_mid = 0.5 * (area.y_max + area.y_min);
  if (area.x_max - x_mid < size_limit || area.y_max - y_mid < size_limit ||
      entities.size() <= population_limit) {
    collision_check(entities, all_collisions);
    return;
  }
  BoundingBox bboxes[4] = {{area.x_min, x_mid, area.y_min, y_mid},
                           {x_mid, area.x_max, area.y_min, y_mid},
                           {area.x_min, x_mid, y_mid, area.y_max},
                           {x_mid, area.x_max, y_mid, area.y_max}};
  for (auto quad : std::views::iota(0) | std::views::take(4)) {
    std::vector<std::shared_ptr<const CollisionEntity>> quad_entities;
    std::ranges::for_each(entities, [&](auto& entity) {
      if (bboxes[quad].overlap(entity->bounding_box)) {
        quad_entities.push_back(entity);
      }
      collision_quad_tree(quad_entities, bboxes[quad], size_limit,
                          population_limit, all_collisions);
    });
  }
}

std::vector<std::shared_ptr<const CollisionEntity>> collect_collision_items(
  const std::set<const CollidingBody*>& colliding_bodies) {
  std::vector<std::shared_ptr<const CollisionEntity>> body_segments;
  std::ranges::for_each(colliding_bodies, [&](auto& body) {
    std::ranges::for_each(
      std::views::iota(0) | std::views::take(body->number_of_segments()),
      [&](auto i) {
        body_segments.push_back(std::make_shared<CollisionEntity>(body, i));
      });
  });
  return body_segments;
}

std::set<CollisionPair> CollisionManager::check_for_collisions(
  const BoundingBox& starting_area) const {
  auto collision_items = collect_collision_items(_colliding_bodies);
  std::set<std::pair<CollisionItem, CollisionItem>> all_collisions;
  collision_quad_tree(collision_items, starting_area, _size_limit,
                      _population_limit, all_collisions);
  return all_collisions;
}

bool CollisionManager::is_occupied(const Shape& test_object) const {
  auto collision_items = collect_collision_items(_colliding_bodies);
  return std::ranges::any_of(collision_items, [&](auto& segment) {
    return test_object.overlap(*segment->shape);
  });
}

}  // namespace VVipers
