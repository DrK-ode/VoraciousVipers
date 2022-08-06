#include <vvipers/Scenes/Collision2/Collider.hpp>

namespace VVipers {

std::unique_ptr<CollisionResult> Collider::collision(
    const Collider& body) const {
    if (!this->isActive() or !body.isActive())
        return std::make_unique<CollisionResult>();
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
    throw std::runtime_error(
        "Unrecognised Collider type (this should not happen).");
}

std::unique_ptr<CollisionResult> Collider::collisionSegmented(
    const ColliderSegmented& segmented) const {
    auto combinedResult = std::make_unique<CollisionResult>();
    auto nextResult = &combinedResult;
    ColliderSegmented::PolygonSegment polygonSegment(segmented);
    ColliderPolygon colliderPolygon(polygonSegment, segmented.isActive());
    for (size_t i = 0; i < segmented.getSegmentCount(); ++i) {
        polygonSegment.setCurrentSegment(i);
        colliderPolygon.setActive(polygonSegment.isActive());
        auto result = collision(colliderPolygon);
        if (result) {
            auto newResult = std::make_unique<CollisionResult>();
            newResult->colliderA = std::make_unique<ColliderPtr>(
                this, std::move(result->colliderA));
            newResult->colliderB = std::make_unique<ColliderPtr>(
                &segmented, std::move(result->colliderB));
            newResult->nextCollision = std::move(result->nextCollision);

            *nextResult = std::move(newResult);
            while (nextResult)
                nextResult = &(*nextResult)->nextCollision;
        }
    }
    return combinedResult;
}

std::unique_ptr<CollisionResult> collisionCirclePolygon(
    const ColliderCircle& circle, const ColliderPolygon& polygon) {
    auto result = std::make_unique<CollisionResult>();
    for (size_t i = 0; i < polygon.getPointCount(); ++i) {
        if (circle.inside(polygon.getGlobalPoint(i))) {
            result->colliderA = std::make_unique<ColliderPtr>(&circle, nullptr);
            result->colliderB =
                std::make_unique<ColliderPtr>(&polygon, nullptr);
            break;
        }
    }
    return result;
}

sf::FloatRect ColliderCircle::getBounds() const {
    auto position = getPosition();
    auto r = getRadius();
    return sf::FloatRect(position.x - r, position.y - r, 2 * r, 2 * r);
}

bool ColliderCircle::inside(Vec2 point) const {
    return (point - this->getPosition()).abs() < this->getRadius();
}

std::unique_ptr<CollisionResult> ColliderCircle::collisionCircle(
    const ColliderCircle& circle) const {
    auto result = std::make_unique<CollisionResult>();
    if ((this != &circle) and (getPosition() - circle.getPosition()).abs() <
                                  getRadius() + circle.getRadius()) {
        result->colliderA = std::make_unique<ColliderPtr>(this, nullptr);
        result->colliderB = std::make_unique<ColliderPtr>(&circle, nullptr);
    }
    return result;
}

std::unique_ptr<CollisionResult> ColliderCircle::collisionPolygon(
    const ColliderPolygon& polygon) const {
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
            (polygon.getGlobalPoint(i) - polygon.getGlobalPoint(i - 1))
                .perpVec());
    axes.push_back((polygon.getGlobalPoint(0) -
                    polygon.getGlobalPoint(polygon.getPointCount() - 1))
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

void swapColliders(std::unique_ptr<CollisionResult>& result) {
    auto temp = std::move(result->colliderA);
    result->colliderA = std::move(result->colliderB);
    result->colliderB = std::move(temp);
}

std::unique_ptr<CollisionResult> ColliderPolygon::collisionCircle(
    const ColliderCircle& circle) const {
    auto result = collisionCirclePolygon(circle, *this);
    // Swap the order so that this object corresponds to the first collider. The
    // function always puts circle first.
    swapColliders(result);
    return result;
}

std::unique_ptr<CollisionResult> ColliderPolygon::collisionPolygon(
    const ColliderPolygon& polygon) const {
    auto result = std::make_unique<CollisionResult>();
    // Rough check before doing it properly
    if ((this == &polygon) or
        (!this->getBounds().intersects(polygon.getBounds())))
        return result;

    const auto axes = this->getPointCount() < polygon.getPointCount()
                          ? getSATAxes(*this)
                          : getSATAxes(polygon);
    for (auto& axis : axes) {
        auto [min1, max1] = projectionsMinMax(*this, axis);
        auto [min2, max2] = projectionsMinMax(polygon, axis);
        if (max1 < min2 || max2 < min1)
            return result;
    }
    result->colliderA = std::make_unique<ColliderPtr>(this, nullptr);
    result->colliderB = std::make_unique<ColliderPtr>(&polygon, nullptr);
    return result;
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
    ColliderPolygon colliderPolygon(polygonSegment, this->isActive());
    for (size_t i = 0; i < this->getSegmentCount(); ++i) {
        polygonSegment.setCurrentSegment(i);
        if (colliderPolygon.inside(point)) {
            return true;
            break;
        }
    }
    return false;
}

std::unique_ptr<CollisionResult> ColliderSegmented::collisionCircle(
    const ColliderCircle& circle) const {
    auto result = circle.collision(*this);
    swapColliders(result);
    return result;
}

std::unique_ptr<CollisionResult> ColliderSegmented::collisionPolygon(
    const ColliderPolygon& polygon) const {
    auto result = polygon.collision(*this);
    swapColliders(result);
    return result;
}

}  // namespace VVipers