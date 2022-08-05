#include <vvipers/Scenes/Collision2/Collider.hpp>

namespace VVipers {

bool Collider::collision(const Collider& body) const {
    switch (body.getType()) {
        case ColliderType::CircleLike: {
            auto& circle = static_cast<const ColliderCircle&>(body);
            return collisionCircle(circle);
            break;
        }
        case ColliderType::Polygon: {
            auto& polygon = static_cast<const ColliderPolygon&>(body);
            return collisionPolygon(polygon);
            break;
        }
        case ColliderType::Segmented: {
            auto& segmented = static_cast<const ColliderSegmented&>(body);
            return collisionSegmented(segmented);
            break;
        }
    }
    throw std::runtime_error("Unrecognised Collider type (this should not happen).");
}

bool Collider::collisionSegmented(const ColliderSegmented& segmented) const {
    for (size_t i = 0; i < segmented.getSegmentCount(); ++i)
        if (collision(segmented.getSegment(i))) {
            return true;
            break;
        }
    return false;
}

bool collisionCirclePolygon(const ColliderCircle& circle,
                            const ColliderPolygon& polygon) {
    for (size_t i = 0; i < polygon.getPointCount(); ++i)
        if (circle.inside(polygon.getGlobalPoint(i))) {
            return true;
            break;
        }
    return false;
}

sf::FloatRect ColliderCircle::getBounds() const {
    auto position = getPosition();
    auto r = getRadius();
    return sf::FloatRect(position.x - r, position.y - r, 2 * r, 2 * r);
}

bool ColliderCircle::inside(Vec2 point) const {
    return (point - this->getPosition()).abs() < this->getRadius();
}

bool ColliderCircle::collisionCircle(const ColliderCircle& circle) const {
    if ((getPosition() - circle.getPosition()).abs() <
        getRadius() + circle.getRadius())
        return true;
    return false;
}

bool ColliderCircle::collisionPolygon(const ColliderPolygon& polygon) const {
    return collisionCirclePolygon(*this, polygon);
}

sf::FloatRect ColliderPolygon::getBounds() const {
    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i < getPointCount(); ++i) {
        auto point = getGlobalPoint(i);
        xmin = std::min(xmin, point.x);
        xmax = std::max(xmax, point.x);
        ymin = std::min(ymin, point.y);
        ymax = std::max(ymax, point.y);
    }
    return sf::FloatRect(xmin, ymin, xmax - xmin, ymax - ymin);
}

/** Helper function for the Separating Axes Theorem algorithm **/
std::tuple<double, double> projectionsMinMax(const ColliderPolygon& polygon,
                                             Vec2 axis) {
    double minimum = std::numeric_limits<double>::max();
    double maximum = std::numeric_limits<double>::lowest();
    for (size_t i = 0; i < polygon.getPointCount(); ++i) {
        double projection = polygon.getGlobalPoint(i).projectionScalar(axis);
        minimum = projection < minimum ? projection : minimum;
        maximum = projection > maximum ? projection : maximum;
    }
    return {minimum, maximum};
}

std::vector<Vec2> getSATAxes(const ColliderPolygon& polygon) {
    std::vector<Vec2> axes;
    axes.reserve(polygon.getPointCount());

    for (size_t i = 1; i < polygon.getPointCount(); ++i)
        axes.push_back(
            (polygon.getGlobalPoint(i) - polygon.getGlobalPoint(i - 1)).perpVec());
    axes.push_back(
        (polygon.getGlobalPoint(0) - polygon.getGlobalPoint(polygon.getPointCount() - 1))
            .perpVec());
    return axes;
}

bool ColliderPolygon::inside(Vec2 point) const {
    for (auto axis : getSATAxes(*this)) {
        auto [minimum, maximum] = projectionsMinMax(*this, axis);
        double projection = point.projectionScalar(axis);
        if (maximum < projection || projection < minimum)
            return false;
    }
    return true;
}

bool ColliderPolygon::collisionCircle(const ColliderCircle& circle) const {
    return collisionCirclePolygon(circle, *this);
}

bool ColliderPolygon::collisionPolygon(const ColliderPolygon& polygon) const {
    // Rough check before doing it properly
    if (!this->getBounds().intersects(polygon.getBounds()))
        return false;

    const auto axes = this->getPointCount() < polygon.getPointCount()
                          ? getSATAxes(*this)
                          : getSATAxes(polygon);
    for (auto& axis : axes) {
        auto [min1, max1] = projectionsMinMax( *this, axis);
        auto [min2, max2] = projectionsMinMax( polygon, axis);
        if (max1 < min2 || max2 < min1)
            return false;
    }
    return true;
}

void combineBounds(sf::FloatRect& bounds, const sf::FloatRect& b) {
    double right = std::max(bounds.left + bounds.width, b.left + b.width);
    double bottom = std::max(bounds.top + bounds.height, b.top + b.height);
    bounds.left = std::min(bounds.left, b.left);
    bounds.top = std::min(bounds.top, b.top);
    bounds.width = right - bounds.left;
    bounds.height = bottom - bounds.top;
}

sf::FloatRect ColliderSegmented::getBounds() const {
    sf::FloatRect bounds;
    for (size_t i = 0; i < getSegmentCount(); ++i) {
        auto b = getSegment(i).getBounds();
        combineBounds(bounds, b);
    }
    return bounds;
}

bool ColliderSegmented::inside(Vec2 point) const {
    for (size_t i = 0; i < getSegmentCount(); ++i) {
        if (getSegment(i).inside(point)) {
            return true;
            break;
        }
    }
    return false;
}

}  // namespace VVipers