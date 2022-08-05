#ifndef VVIPERS_SCENES_COLLISION2_SHAPEWRAPPER_HPP
#define VVIPERS_SCENES_COLLISION2_SHAPEWRAPPER_HPP

#include <SFML/Graphics/Shape.hpp>

namespace VVipers
{
    
class ShapeWrapper : public sf::Shape {
  public:
    ShapeWrapper(sf::Shape& shape) : sf::Shape(shape) {}
    Vec2 getPointGlobal(size_t i) const {
        getTransform().transformPoint(getPoint(i));
    }
}

} // namespace VVipers

#endif // VVIPERS_SCENES_COLLISION2_SHAPEWRAPPER_HPP
