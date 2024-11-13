#ifndef VVIPERS_COLLISIONS_COLLIDING_SEGMENT_HPP
#define VVIPERS_COLLISIONS_COLLIDING_SEGMENT_HPP

#include <sys/types.h>

#include <SFML/Graphics/Rect.hpp>

#include "vvipers/Utilities/Vec2.hpp"
namespace VVipers {

enum class CollidingSegmentType {
    Circle,
    Polygon,
};

class CollidingBody;
class CollidingCircle;
class CollidingPolygon;

class CollidingSegment {
  public:
    CollidingSegment(const CollidingBody* owner, CollidingSegmentType type,
                     sf::FloatRect bounding_box, uint segment_id)
        : _owner(owner),
          _type(type),
          _bounding_box(bounding_box),
          _segment_id(segment_id) {}
    virtual ~CollidingSegment() {}
    virtual const CollidingBody* base_object() const { return _owner; };
    const sf::FloatRect& bounding_box() const { return _bounding_box; }
    virtual bool collision(const CollidingSegment&) const = 0;
    uint segment_id() const { return _segment_id; }
    virtual CollidingSegmentType type() const { return _type; }

  private:
    const CollidingBody* _owner;
    const CollidingSegmentType _type;
    const sf::FloatRect _bounding_box;
    const uint _segment_id;
};

class CollidingCircle : public CollidingSegment {
  public:
    CollidingCircle(const Vec2& center, double radius,
                    const CollidingBody* owner, uint segment_id = 0)
        : CollidingSegment(owner, CollidingSegmentType::Circle,
                           sf::FloatRect(center.x - radius, center.y - radius,
                                         2 * radius, 2 * radius),
                           segment_id),
          _center(center),
          _radius(radius) {}
    const Vec2& center() const { return _center; }
    bool collision(const CollidingSegment&) const override;
    bool collision(const CollidingCircle&) const;
    std::tuple<double, double> min_max_projections(const Vec2& axis) const;
    double radius() const { return _radius; }

  private:
    const Vec2 _center;
    const double _radius;
};

class CollidingPolygon : public CollidingSegment {
  public:
    CollidingPolygon(const std::vector<Vec2>& corners,
                     const CollidingBody* owner, uint segment_id = 0)
        : CollidingSegment(owner, CollidingSegmentType::Polygon,
                           calculate_bounding_box(corners), segment_id),
          _corners(corners) {}
    bool collision(const CollidingSegment&) const override;
    bool collision(const CollidingCircle&) const;
    bool collision(const CollidingPolygon&) const;
    const std::vector<Vec2> corners() const { return _corners; }
    std::tuple<double, double> min_max_projections(const Vec2& axis) const;
    std::vector<Vec2> normal_vectors() const;

  private:
    static sf::FloatRect calculate_bounding_box(const std::vector<Vec2>&);
    const std::vector<Vec2> _corners;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISIONS_COLLIDING_SEGMENT_HPP