#include "vvipers/Collisions/CollidingSegment.hpp"

#include <SFML/Graphics/Rect.hpp>
namespace VVipers {

bool CollidingCircle::collision(const CollidingSegment& other) const {
    switch (other.type()) {
        case CollidingSegmentType::Circle: {
            return this->collision(reinterpret_cast<const CollidingCircle&>(other));
        }
        case VVipers::CollidingSegmentType::Polygon: {
            return reinterpret_cast<const CollidingPolygon&>(other).collision(*this);
        }
    }
}

bool CollidingCircle::collision(const CollidingCircle& other) const {
    double r = (this->_radius + other._radius);
    return (this->_center - other._center).squared() < r * r;
}

sf::FloatRect CollidingPolygon::calculate_bounding_box(
    const std::vector<Vec2>& corners) {
    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < corners.size(); ++i) {
        const Vec2& point = corners[i];
        xmin = std::min(xmin, point.x);
        xmax = std::max(xmax, point.x);
        ymin = std::min(ymin, point.y);
        ymax = std::max(ymax, point.y);
    }
    return sf::FloatRect(xmin, ymin, xmax - xmin, ymax - ymin);
}

std::vector<Vec2> CollidingPolygon::normal_vectors() const {
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

std::tuple<double, double> CollidingCircle::min_max_projections(
    const Vec2& axis) const {
    auto proj = this->_center.projectionScalar(axis);
    double maximum = proj + this->_radius;
    double minimum = proj - this->_radius;
    return {minimum, maximum};
}

std::tuple<double, double> CollidingPolygon::min_max_projections(
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

bool CollidingPolygon::collision(const CollidingSegment& other) const {
    switch (other.type()) {
        case CollidingSegmentType::Circle: {
            return this->collision(reinterpret_cast<const CollidingCircle&>(other));
        }
        case VVipers::CollidingSegmentType::Polygon: {
            return this->collision(reinterpret_cast<const CollidingPolygon&>(other));
        }
    }
}

bool CollidingPolygon::collision(const CollidingCircle& circle) const {
    if (!this->bounding_box().intersects(circle.bounding_box()))
        return false;
    const auto axes = this->normal_vectors();
    for (auto& axis : axes) {
        auto [poly_min, poly_max] = this->min_max_projections(axis);
        auto [circ_min, circ_max] = circle.min_max_projections(axis);
        // If the two projections are not overlapping, there cannot be a
        // collision.
        if (poly_max < circ_min || circ_max < poly_min)
            return false;
    }
    return true;
}

bool CollidingPolygon::collision(const CollidingPolygon& other) const {
    if (!this->bounding_box().intersects(other.bounding_box()))
        return false;

    const auto axes = this->_corners.size() < other._corners.size()
                          ? this->normal_vectors()
                          : other.normal_vectors();
    for (auto& axis : axes) {
        auto [min1, max1] = this->min_max_projections(axis);
        auto [min2, max2] = other.min_max_projections(axis);
        if (max1 < min2 || max2 < min1)
            return false;
    }
    return true;
}

}  // namespace VVipers