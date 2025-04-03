#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <algorithm>
#include <memory>
#include <vector>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/Utilities/VVColor.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>
#include <vvipers/config.hpp>

#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/GameElements/Track.hpp"
#include "vvipers/Utilities/TriangleStripArray.hpp"

namespace VVipers {

using namespace std::chrono_literals;

Viper::Viper(std::shared_ptr<const ViperConfiguration> configuration,
             const Vec2& tail_position, double angle,
             double number_of_body_segments)
  : CollidingBody("Viper"),
    _viper_configuration(configuration),
    _angular_speed(0),
    _boost_increase(0.),
    _boost_charge(0),
    _boost_recharge_cooldown(0.),
    _growth(0.) {
  _boost_charge = _viper_configuration->boost_max_charge;
  _nominalSpeed = _viper_configuration->nominal_speed;
  _speed = _nominalSpeed;
  _angle = angle;
  _temporal_length =
    _viper_configuration->head_duration + _viper_configuration->tail_duration;
  _growth = number_of_body_segments * _viper_configuration->body_duration;

  Vec2 direction = Vec2(1, 0).rotate(angle);
  double length = time_as_seconds(_temporal_length) * speed();
  auto viper_vector = length * direction;
  _track = std::unique_ptr<TemporalTrack>(
    new TemporalTrack(tail_position + viper_vector, _temporal_length,
                      tail_position, time_from_seconds(0)));
  _triangle_strip_head.texture = _viper_configuration->head_texture;
  _triangle_strip_body.texture = _viper_configuration->body_texture;
  _triangle_strip_tail.texture = _viper_configuration->tail_texture;
  update_vertices_and_polygons();
}

void Viper::eat(const Food& food) {
  add_growth(_viper_configuration->body_duration *
               (food.getRadius() * food.getRadius()) /
               (Food::nominal_food_radius * Food::nominal_food_radius),
             _track->head_time(), food.color());
  add_boost_charge(0.5s);
}

void Viper::die(const Time& elapsed_time) {
  _temporal_length -= 4 * elapsed_time;
  if (_temporal_length <= time_from_seconds(0))
    state(Dead);
}

void Viper::add_growth(Time howMuch, Time when, sf::Color color) {
  _dinner_times[when] = {howMuch, color};
}

double Viper::length() const {
  return _track->length(_track->head_time(),
                        _track->head_time() - _temporal_length);
}

void Viper::create_next_head_temporal_track_point(Time elapsed_time) {
  _track->create_front(Vec2(_speed, 0).rotate(_angle), elapsed_time);
}

void Viper::clean_up_trailing_temporal_track_points() {
  // Removes any point(s) the whole Viper has passed through
  _track->remove_trailing(_track->head_time() - _temporal_length);
}

void Viper::clean_up_dinner_times() {
  // Slightly wasteful but there will never be more than a handful dinner
  // times stored at the same time.
  for (auto& dinnerTime : _dinner_times)
    if (dinnerTime.first < _track->tail_time()) {
      _growth += dinnerTime.second.amount;
      dinnerTime.second.amount = time_from_seconds(0);
    }
  // This loop can only remove a maximum of one time per update. But that's
  // fine
  for (auto& dinnerTime : _dinner_times)
    // Times 10 give us some margin but it would be nicer to specify
    // exactly...
    if (dinnerTime.first + 10 * _viper_configuration->body_duration <
        _track->tail_time()) {
      _dinner_times.erase(dinnerTime.first);
      break;
    }
}

Time Viper::boost_max() const { return _viper_configuration->boost_max_charge; }

void Viper::grow(const Time& elapsed_time) {
  // Limit the growth to how much time that has passed
  Time actualGrowth = std::min(_growth, elapsed_time);
  _temporal_length += actualGrowth;
  _growth -= actualGrowth;
}

// This allows the viper narrower turns when not boosting
double Viper::max_angular_speed() const {
  return _nominalSpeed / _viper_configuration->nominal_segment_width;
}

void Viper::update(Time elapsed_time) {
  if (state() == Dead) {
    notify(DestroyEvent(this));
    return;
  }
  if (state() == Dying) {
    die(elapsed_time);  // Allow the viper to die for a while
  } else {
    create_next_head_temporal_track_point(elapsed_time);
    grow(elapsed_time);
  }
  update_motion(elapsed_time);
  update_vertices_and_polygons();
  clean_up_trailing_temporal_track_points();
  clean_up_dinner_times();
}

void Viper::add_boost_charge(Time charge) {
  auto oldCharge = _boost_charge;
  _boost_charge += charge;
  _boost_charge = std::max(_boost_charge, time_from_seconds(0));
  _boost_charge = std::min(_boost_charge, boost_max());

  if (_boost_charge != oldCharge) {
    notify(ObjectModifiedEvent(this));
  }
}

void Viper::update_boost_charge(Time elapsed_time) {
  if (_boost_recharge_cooldown == time_from_seconds(0)) {
    add_boost_charge(elapsed_time * _viper_configuration->boost_recharge_rate);
  } else if (_boost_increase > 0)
    add_boost_charge(-elapsed_time);
}

void Viper::update_speed(const Time& elapsed_time) {
  double acceleration = 0;
  double targetSpeed = _viper_configuration->nominal_speed;
  if (_boost_increase > 0) {
    targetSpeed *= (1 + _boost_increase);
    _boost_recharge_cooldown = _viper_configuration->boost_recharge_cooldown;
  } else {
    _boost_recharge_cooldown -=
      std::min(_boost_recharge_cooldown, elapsed_time);
  }
  update_boost_charge(elapsed_time);
  if (_speed < targetSpeed) {
    // 0.5s to increase speed by nominal speed but cap at targetSpeed
    acceleration =
      std::min(2 * _viper_configuration->nominal_speed,
               (targetSpeed - _speed) / time_as_seconds(elapsed_time));
  } else if (_speed > targetSpeed) {
    acceleration =
      std::max(-_viper_configuration->nominal_speed,
               (targetSpeed - _speed) / time_as_seconds(elapsed_time));
  }
  _speed += acceleration * time_as_seconds(elapsed_time);
}

void Viper::update_angle(const Time& elapsed_time) {
  _angle += _angular_speed * time_as_seconds(elapsed_time);
}

void Viper::update_motion(const Time& elapsed_time) {
  update_speed(elapsed_time);
  update_angle(elapsed_time);
}

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  _triangle_strip_head.draw(target, states);
  _triangle_strip_body.draw(target, states);
  _triangle_strip_tail.draw(target, states);
}

void Viper::create_vertex_vectors_and_polygons_for_body_part(
  ViperPart viper_part, const Time& part_start, const Time& part_duration) {
  if (part_duration <= time_from_seconds(0))
    return;

  TriangleStripArray* vertex_vector;
  const std::vector<Vec2>* nodes;
  size_t number_of_segments = 1;
  Time nominal_segment_duration;
  switch (viper_part) {
    case ViperPart::Head: {
      nominal_segment_duration = _viper_configuration->head_duration;
      nodes = &_viper_configuration->head_nodes;
      vertex_vector = &_triangle_strip_head;
      break;
    }
    case ViperPart::Body: {
      number_of_segments =
        part_duration / _viper_configuration->body_duration + 1;
      nominal_segment_duration = _viper_configuration->body_duration;
      nodes = &_viper_configuration->body_nodes;
      vertex_vector = &_triangle_strip_body;
      break;
    }
    case ViperPart::Tail: {
      nominal_segment_duration = _viper_configuration->tail_duration;
      nodes = &_viper_configuration->tail_nodes;
      vertex_vector = &_triangle_strip_tail;
      break;
    }
  }

  Vec2 texture_size = vertex_vector->texture->getSize();
  for (size_t segment_index = 0; segment_index < number_of_segments;
       ++segment_index) {
    size_t nodes_shared_with_prev_segment = segment_index == 0 ? 0 : 1;
    Time segment_start = part_start - segment_index * nominal_segment_duration;
    Time segment_duration =
      std::min(nominal_segment_duration,
               part_duration - segment_index * nominal_segment_duration);

    for (size_t node_index = nodes_shared_with_prev_segment;
         node_index < nodes->size(); ++node_index) {
      Time time = segment_start - segment_duration * (*nodes)[node_index].y;
      Vec2 position = _track->position(time);
      Vec2 velocity = _track->velocity(time);
      Vec2 width = velocity.perpendicular() *
                   _viper_configuration->nominal_speed *
                   _viper_configuration->nominal_segment_width *
                   (*nodes)[node_index].x / velocity.dot(velocity);

      Vec2 texture_coordinates_right =
        (Vec2(0.5f, segment_index) + (*nodes)[node_index]) * texture_size;
      Vec2 texture_coordinates_left =
        Vec2(texture_size.x - texture_coordinates_right.x,
             texture_coordinates_right.y);

      sf::Color color = calculate_vertex_color(time);

      vertex_vector->vertices.emplace_back(position + width, color,
                                           texture_coordinates_right);
      vertex_vector->vertices.emplace_back(position - width, color,
                                           texture_coordinates_left);
    }
  }
  std::ranges::for_each(
    vertex_vector->create_polygons(number_of_segments), [this](auto& polygon) {
      _polygons.push_back(std::make_shared<Polygon>(std::move(polygon)));
    });
}

void Viper::update_vertices_and_polygons() {
  Time head_duration =
    std::min(_temporal_length, _viper_configuration->head_duration);
  if (_temporal_length < _viper_configuration->head_duration +
                           _viper_configuration->tail_duration) {
    head_duration = _temporal_length * (_viper_configuration->head_duration /
                                        (_viper_configuration->head_duration +
                                         _viper_configuration->tail_duration));
  }
  Time tail_duration = std::min(_temporal_length - head_duration,
                                _viper_configuration->tail_duration);
  Time body_duration = _temporal_length - head_duration - tail_duration;

  Time head_start = _track->head_time();
  Time body_start = head_start - head_duration;
  Time tail_start = body_start - body_duration;

  _polygons.clear();
  _triangle_strip_head.vertices.clear();
  _triangle_strip_body.vertices.clear();
  _triangle_strip_tail.vertices.clear();
  create_vertex_vectors_and_polygons_for_body_part(ViperPart::Head, head_start,
                                                   head_duration);
  create_vertex_vectors_and_polygons_for_body_part(ViperPart::Body, body_start,
                                                   body_duration);
  create_vertex_vectors_and_polygons_for_body_part(ViperPart::Tail, tail_start,
                                                   tail_duration);
}

sf::Color Viper::calculate_vertex_color(Time time) {
  // TODO: Loop through DinnerTimes instead of calling for every vertex.

  auto headSide = _dinner_times.lower_bound(time);  // Closer to head
  auto tailSide = headSide != _dinner_times.cbegin() ? std::prev(headSide)
                                                     : _dinner_times.cend();

  double sFactor1 = 0;
  sf::Color color1 = sf::Color::Transparent;
  if (tailSide != _dinner_times.end()) {
    Time half_length = tailSide->second.amount;
    Time mid_point = tailSide->first + 0.5 * half_length;
    Time distance = std::chrono::abs(time - mid_point);
    if (distance < 0.5 * half_length) {
      color1 = tailSide->second.color;
      sFactor1 = 1. - distance / half_length;
    }
  }
  double sFactor2 = 0;
  sf::Color color2 = sf::Color::Transparent;
  if (headSide != _dinner_times.end()) {
    Time half_length = headSide->second.amount;
    Time mid_point = headSide->first + 0.5 * half_length;
    Time distance = std::chrono::abs(time - mid_point);
    if (distance < 0.5 * half_length) {
      color1 = headSide->second.color;
      sFactor1 = 1. - distance / half_length;
    }
  }
  double sumFactor = sFactor1 + sFactor2;
  if (sumFactor > 1) {
    sFactor1 = sFactor1 / sumFactor;
    sFactor2 = sFactor2 / sumFactor;
  }
  // Blends the main color with the blend of the two food colors (normally
  // only one will be present)
  return blend_colors(_primaryColor, 1. - sumFactor,
                      blend_colors(color1, sFactor1, color2, sFactor2),
                      sumFactor);
}

}  // namespace VVipers
