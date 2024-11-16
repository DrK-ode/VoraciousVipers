#ifndef VVIPERS_COLLISIONS_COLLIDING_BODY_HPP
#define VVIPERS_COLLISIONS_COLLIDING_BODY_HPP

#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <vvipers/Utilities/Vec2.hpp>

#include "vvipers/Utilities/Shape.hpp"

namespace VVipers {

class CollidingBody {
  public:
    virtual ~CollidingBody() {}
    virtual size_t number_of_body_parts() const = 0;
    virtual std::shared_ptr<const Shape> body_part_shape(size_t index) const = 0;
    bool operator==(const CollidingBody& other) const{
        return this == &other;
    }
};

}  // Namespace VVipers

#endif  // VVIPERS_COLLISIONS_COLLIDING_BODY_HPP