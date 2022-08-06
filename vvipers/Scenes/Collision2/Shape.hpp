#ifndef VVIPERS_SCENES_COLLISION2_SHAPE_HPP
#define VVIPERS_SCENES_COLLISION2_SHAPE_HPP

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vvipers/Scenes/Collision2/Collider.hpp>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

/** These classes adds the methode getGlobalPoint(size_t) to the SFML Shape
 * classes so that Collider instances can be based off them **/

class CircleShape : public sf::CircleShape, public ColliderCircle {
  public:
    CircleShape(double radius = 0, size_t pointCount = 30, bool active = false)
        : sf::CircleShape(radius, pointCount), ColliderCircle(active) {
            setRadius(radius);
        }
    Vec2 getGlobalPoint(size_t i) const {
        return getTransform().transformPoint(getPoint(i));
    }
    Vec2 getPosition() const override { return sf::CircleShape::getPosition(); }
    double getRadius() const override { return sf::CircleShape::getRadius(); }
    void setRadius(double r) {
        sf::CircleShape::setRadius(r);
        setOrigin(r, r);
    }
};

class ConvexShape : public sf::ConvexShape, public ColliderPolygon {
  public:
    ConvexShape(size_t pointCount = 0, bool active = false)
        : sf::ConvexShape(pointCount), ColliderPolygon(active) {}
    Vec2 getGlobalPoint(size_t i) const {
        return getTransform().transformPoint(getPoint(i));
    }
    size_t getPointCount() const override {
        return sf::ConvexShape::getPointCount();
    }
};

class RectangleShape : public sf::RectangleShape, public ColliderPolygon {
  public:
    RectangleShape(const Vec2& size = Vec2(0., 0.), bool active = false)
        : sf::RectangleShape(sf::Vector2f(size)), ColliderPolygon(active) {}
    Vec2 getGlobalPoint(size_t i) const {
        return getTransform().transformPoint(getPoint(i));
    }
    size_t getPointCount() const override {
        return sf::RectangleShape::getPointCount();
    }
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_COLLISION2_SHAPE_HPP
