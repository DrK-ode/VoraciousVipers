#pragma once

#include "vvipers/Utilities/Vec2.hpp"

namespace VVipers {
enum class ShapeType {
    Circle,
    Polygon,
};

class BoundingBox {
  public:
    BoundingBox(double x1, double x2, double y1, double y2)
        : x_min(x1), x_max(x2), y_min(y1), y_max(y2) {}
    bool overlap(const BoundingBox& other) const {
        return !(this->x_min > other.x_max || this->x_max < other.x_min ||
                 this->y_min > other.y_max || this->y_max < other.y_min);
    }
    double x_min, x_max, y_min, y_max;
};

class Shape {
  public:
    Shape(ShapeType type) : _type(type) {}
    virtual ~Shape() {}
    virtual BoundingBox bounding_box() const = 0;
    virtual void move_to(const Vec2&) = 0;
    virtual bool overlap(const Shape&) const = 0;
    virtual void rotate(double) = 0;
    virtual std::tuple<double, double> projection_on_vector(
        const Vec2&) const = 0;
    virtual ShapeType type() const { return _type; }

  private:
    ShapeType _type;
};

class Circle : public Shape {
  public:
    Circle(const Vec2& center, double radius)
        : Shape(ShapeType::Circle), _center(center), _radius(radius) {}
    BoundingBox bounding_box() const override {
        return {_center.x - _radius, _center.x + _radius, _center.y - _radius,
                _center.y + _radius};
    }
    const Vec2& center() const { return _center; }
    void move_to(const Vec2& new_center) override { _center = new_center; };
    bool overlap(const Shape&) const override;
    std::tuple<double, double> projection_on_vector(const Vec2&) const override;
    double radius() const { return _radius; }
    void rotate(double) override {};

  private:
    Vec2 _center;
    double _radius;
};

class Polygon : public Shape {
  public:
    Polygon(const Vec2& anchor, const std::vector<Vec2>& corners)
        : Shape(ShapeType::Polygon), _anchor(anchor), _corners(corners) {}
    Polygon(const std::vector<Vec2>& corners);
    BoundingBox bounding_box() const override;
    const Vec2& anchor() const { return _anchor; }
    const std::vector<Vec2>& corners() const { return _corners; }
    void move_to(const Vec2& new_center) override { _anchor = new_center; }
    std::vector<Vec2> normal_vectors() const;
    bool overlap(const Shape&) const override;
    std::tuple<double, double> projection_on_vector(const Vec2&) const override;
    void rotate(double) override;

  private:
    Vec2 _anchor;
    std::vector<Vec2> _corners;
};
}  // namespace VVipers