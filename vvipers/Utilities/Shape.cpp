#include "vvipers/Utilities/Shape.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include "vvipers/Utilities/debug.hpp"

namespace VVipers {

std::tuple<double, double> Circle::projection_on_vector(
    const Vec2& axis) const {
    auto proj = this->_center.projectionScalar(axis);
    double maximum = proj + this->_radius;
    double minimum = proj - this->_radius;
    return {minimum, maximum};
}

bool Circle::overlap(const Shape& other) const {
    switch (other.type()) {
        case ShapeType::Circle: {
            const Circle& other_circle = reinterpret_cast<const Circle&>(other);
            double r = (this->_radius + other_circle._radius);
            return (this->_center - other_circle._center).squared() < r * r;
        }
        case ShapeType::Polygon: {
            return reinterpret_cast<const Polygon&>(other).overlap(*this);
        }
    }
}

Polygon::Polygon(const std::vector<Vec2>& corners)
    : Shape(ShapeType::Polygon), _corners(corners) {
    BoundingBox box = this->bounding_box();
    _anchor = Vec2(box.x_max - box.x_min, box.y_max - box.y_min);
}

BoundingBox Polygon::bounding_box() const {
    double x_min = std::numeric_limits<double>::max();
    double y_min = std::numeric_limits<double>::max();
    double x_max = std::numeric_limits<double>::lowest();
    double y_max = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < _corners.size(); ++i) {
        const Vec2& point = _corners[i];
        x_min = std::min(x_min, point.x);
        x_max = std::max(x_max, point.x);
        y_min = std::min(y_min, point.y);
        y_max = std::max(y_max, point.y);
    }
    return {x_min, x_max, y_min, y_max};
}

std::vector<Vec2> Polygon::normal_vectors() const {
    std::vector<Vec2> axes;
    size_t n_corners = this->_corners.size();
    axes.reserve(n_corners);

    for (size_t i = 1; i < n_corners; ++i)
        axes.push_back(
            (this->_corners[i] - this->_corners[i - 1]).perpendicular());
    axes.push_back(
        (this->_corners[0] - this->_corners[n_corners - 1]).perpendicular());
    return axes;
}

bool Polygon::overlap(const Shape& other) const {
    if (!this->bounding_box().overlap(other.bounding_box()))
        return false;
    switch (other.type()) {
        case ShapeType::Circle: {
            const Circle& other_circle = reinterpret_cast<const Circle&>(other);
            const auto axes = this->normal_vectors();
            for (auto& axis : axes) {
                auto [poly_min, poly_max] = this->projection_on_vector(axis);
                auto [circ_min, circ_max] =
                    other_circle.projection_on_vector(axis);
                // If the two projections are not overlapping, there cannot be a
                // collision.
                if (poly_max <= circ_min || circ_max <= poly_min)
                    return false;
            }
            return true;
        }
        case ShapeType::Polygon: {
            const Polygon& other_polygon =
                reinterpret_cast<const Polygon&>(other);
            const auto axes =
                this->_corners.size() < other_polygon._corners.size()
                    ? this->normal_vectors()
                    : other_polygon.normal_vectors();
            for (auto& axis : axes) {
                auto [min1, max1] = this->projection_on_vector(axis);
                auto [min2, max2] = other.projection_on_vector(axis);
                if (max1 <= min2 || max2 <= min1)
                    return false;
            }
            return true;
        }
    }
}

std::tuple<double, double> Polygon::projection_on_vector(
    const Vec2& axis) const {
    double minimum = std::numeric_limits<double>::max();
    double maximum = std::numeric_limits<double>::lowest();
    size_t n_corners = this->_corners.size();
    for (size_t i = 0; i < n_corners; ++i) {
        double projection = this->_corners[i].projectionScalar(axis);
        minimum = std::min(projection, minimum);
        maximum = std::max(projection, maximum);
    }
    return {minimum, maximum};
}

void Polygon::rotate(double rads) {
    for (Vec2& corner : _corners) {
        corner = _anchor + (corner - _anchor).rotate(rads);
    }
}

std::vector<sf::Vertex> Polygon::triangle_strip(sf::Color color) const {
    std::vector<sf::Vertex> vertices;
    for (size_t i = 0; i < _corners.size(); ++i) {
        size_t index = i % 2 == 0 ? i / 2 : _corners.size() - (i + 1) / 2;
        vertices.emplace_back((sf::Vector2f)_corners[index], color);
    }
    return vertices;
}

}  // namespace VVipers