#ifndef VVIPERS_SCENES_COLLISION2_SHAPE_HPP
#define VVIPERS_SCENES_COLLISION2_SHAPE_HPP

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

/** These classes adds the methode getPointGlobal(size_t) to the SFML Shape
 * classes so that Collider instances can be based off them **/

class CircleShape : public sf::CircleShape {
  public:
    CircleShape(float radius = 0, size_t pointCount = 30)
        : sf::CircleShape(radius, pointCount) {}
    Vec2 getPointGlobal(size_t i) const {
        getTransform().transformPoint(getPoint(i));
    }
};

class ConvexShape : public sf::ConvexShape {
  public:
    ConvexShape(size_t pointCount = 0) : sf::ConvexShape(pointCount) {}
    Vec2 getPointGlobal(size_t i) const {
        getTransform().transformPoint(getPoint(i));
    }
};

class RectangleShape : public sf::RectangleShape {
  public:
    RectangleShape(const Vec2& size = Vec2(0., 0.))
        : sf::RectangleShape(size) {}
    Vec2 getPointGlobal(size_t i) const {
        getTransform().transformPoint(getPoint(i));
    }
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_COLLISION2_SHAPE_HPP
