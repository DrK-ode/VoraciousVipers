#include <vvipers/Collision/Collider.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

CollisionVector Collider::collision(const Collider& body1,
                                    const Collider& body2) {
    CollisionVector result;
    if (!body1.isActive() and !body2.isActive())
        return result;
    switch (body1.getType()) {
        case ColliderType::CircleLike: {
            auto& circle1 = static_cast<const ColliderCircle&>(body1);
            switch (body2.getType()) {
                case ColliderType::CircleLike: {
                    auto& circle2 = static_cast<const ColliderCircle&>(body2);
                    auto r = collisionCircleCircle(circle1, circle2);
                    if (r)
                        result.push_back(r);
                    break;
                }
                case ColliderType::Polygon: {
                    auto& polygon2 = static_cast<const ColliderPolygon&>(body2);
                    auto r = collisionCirclePolygon(circle1, polygon2);
                    if (r)
                        result.push_back(r);
                    break;
                }
                case ColliderType::Segmented: {
                    result = collision(body2, body1);
                    break;
                }
            }
            break;
        }
        case ColliderType::Polygon: {
            auto& polygon1 = static_cast<const ColliderPolygon&>(body1);
            switch (body2.getType()) {
                case ColliderType::CircleLike: {
                    auto& circle2 = static_cast<const ColliderCircle&>(body2);
                    auto r = collisionCirclePolygon(circle2, polygon1);
                    if (r)
                        result.push_back(r);
                    break;
                }
                case ColliderType::Polygon: {
                    auto& polygon2 = static_cast<const ColliderPolygon&>(body2);
                    auto r = collisionPolygonPolygon(polygon1, polygon2);
                    if (r)
                        result.push_back(r);
                    break;
                }
                case ColliderType::Segmented: {
                    result = collision(body2, body1);
                    break;
                }
            }
            break;
        }
        case ColliderType::Segmented: {
            auto& segmented1 = static_cast<const ColliderSegmented&>(body1);
            if (body2.getType() == ColliderType::Segmented) {
                auto& segmented2 = static_cast<const ColliderSegmented&>(body2);
                result = collisionBothSegmented(segmented1, segmented2);
            } else
                result = collisionSegmented(segmented1, body2);
            break;
        }
    }
    return result;
}

CollisionVector Collider::collisionBothSegmented(
    const ColliderSegmented& segmented1, const ColliderSegmented& segmented2) {
    CollisionVector combinedResult;

    ColliderSegmented::PolygonSegment polygonSegment1(segmented1);
    ColliderSegmented::PolygonSegment polygonSegment2(segmented2);

    for (size_t i = 0; i < segmented1.getSegmentCount(); ++i) {
        polygonSegment1.setCurrentSegment(i);
        polygonSegment1.setActive(segmented1.isSegmentActive(i));
        size_t startIndex2 = &segmented1 == &segmented2 ? i + 1 : 0;
        for (size_t j = startIndex2; j < segmented2.getSegmentCount(); ++j) {
            polygonSegment2.setCurrentSegment(j);
            polygonSegment2.setActive(segmented2.isSegmentActive(j));

            auto result =
                collisionPolygonPolygon(polygonSegment1, polygonSegment2);
            if (result) {
                result.m_colliderSegmentA.m_collider = &segmented1;
                result.m_colliderSegmentA.m_segmentIndex = i;
                result.m_colliderSegmentB.m_collider = &segmented2;
                result.m_colliderSegmentB.m_segmentIndex = j;
                combinedResult.push_back(result);
            }
        }
    }
    return combinedResult;
}

CollisionVector Collider::collisionSegmented(
    const ColliderSegmented& segmented1, const Collider& body2) {
    CollisionVector combinedResult;
    ColliderSegmented::PolygonSegment polygonSegment(segmented1);

    for (size_t i = 0; i < segmented1.getSegmentCount(); ++i) {
        polygonSegment.setCurrentSegment(i);
        polygonSegment.setActive(segmented1.isSegmentActive(i));
        CollisionResult result;
        if (body2.getType() == ColliderType::CircleLike)
            result = collisionCirclePolygon(
                static_cast<const ColliderCircle&>(body2), polygonSegment);
        else
            result = collisionPolygonPolygon(
                static_cast<const ColliderPolygon&>(body2), polygonSegment);
        if (result) {
            result.m_colliderSegmentB.m_collider = &segmented1;
            result.m_colliderSegmentB.m_segmentIndex = i;
            combinedResult.push_back(result);
        }
    }
    return combinedResult;
}

sf::FloatRect ColliderCircle::getBounds() const {
    auto position = getPosition();
    auto r = getRadius();
    return sf::FloatRect(position.x - r, position.y - r, 2 * r, 2 * r);
}

bool ColliderCircle::inside(Vec2 point) const {
    return (point - this->getPosition()).squared() < this->getRadius()*this->getRadius();
}

CollisionResult Collider::collisionCircleCircle(const ColliderCircle& circle1,
                                                const ColliderCircle& circle2) {
    CollisionResult result;
    if ((&circle1 != &circle2) and
        (circle1.getPosition() - circle2.getPosition()).abs() <
            circle1.getRadius() + circle2.getRadius()) {
        result.m_colliderSegmentA.m_collider = &circle1;
        result.m_colliderSegmentB.m_collider = &circle2;
    }
    return result;
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

/** Helper function for the Separating Axes Theorem
 * algorithm **/
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
            (polygon.getGlobalPoint(i) - polygon.getGlobalPoint(i - 1))
                .perpendicular());
    axes.push_back((polygon.getGlobalPoint(0) -
                    polygon.getGlobalPoint(polygon.getPointCount() - 1))
                       .perpendicular());
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

CollisionResult Collider::collisionCirclePolygon(
    const ColliderCircle& circle, const ColliderPolygon& polygon) {
    CollisionResult result;
    // Rough check before doing it properly

    if ((!circle.getBounds().intersects(polygon.getBounds())))
        return result;

    const auto axes = getSATAxes(polygon);
    for (auto& axis : axes) {
        auto [min1, max1] = projectionsMinMax(polygon, axis);
        auto proj = circle.getPosition().projectionScalar(axis);
        double max2 = proj + circle.getRadius();
        double min2 = proj - circle.getRadius();
        if (max1 < min2 || max2 < min1)
            return result;
    }
    result.m_colliderSegmentA.m_collider = &circle;
    result.m_colliderSegmentB.m_collider = &polygon;
    return result;
}

CollisionResult Collider::collisionPolygonPolygon(
    const ColliderPolygon& polygon1, const ColliderPolygon& polygon2) {
    CollisionResult result;
    // Rough check before doing it properly

    if ((&polygon1 == &polygon2) or
        (!polygon1.getBounds().intersects(polygon2.getBounds())))
        return result;

    const auto axes = polygon1.getPointCount() < polygon2.getPointCount()
                          ? getSATAxes(polygon1)
                          : getSATAxes(polygon2);
    for (auto& axis : axes) {
        auto [min1, max1] = projectionsMinMax(polygon1, axis);
        auto [min2, max2] = projectionsMinMax(polygon2, axis);
        if (max1 < min2 || max2 < min1)
            return result;
    }
    result.m_colliderSegmentA.m_collider = &polygon1;
    result.m_colliderSegmentB.m_collider = &polygon2;
    return result;
}

sf::FloatRect ColliderSegmented::getBounds() const {
    double xmin = std::numeric_limits<double>::max();
    double ymin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::lowest();
    double ymax = std::numeric_limits<double>::lowest();
    for (size_t i = 0; i < this->getSegmentCount(); ++i)
        for (size_t j = 0; j < getSegmentPointCount(i); ++j) {
            auto point = getSegmentGlobalPoint(i, j);
            xmin = std::min(xmin, point.x);
            xmax = std::max(xmax, point.x);
            ymin = std::min(ymin, point.y);
            ymax = std::max(ymax, point.y);
        }
    return sf::FloatRect(xmin, ymin, xmax - xmin, ymax - ymin);
}

bool ColliderSegmented::inside(Vec2 point) const {
    ColliderSegmented::PolygonSegment polygonSegment(*this);
    for (size_t i = 0; i < this->getSegmentCount(); ++i) {
        polygonSegment.setCurrentSegment(i);
        if (polygonSegment.inside(point)) {
            return true;
            break;
        }
    }
    return false;
}

}  // namespace VVipers