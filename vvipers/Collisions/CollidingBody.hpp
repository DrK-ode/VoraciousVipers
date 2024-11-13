#ifndef VVIPERS_COLLISIONS_COLLIDING_BODY_HPP
#define VVIPERS_COLLISIONS_COLLIDING_BODY_HPP

#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <vvipers/Utilities/Vec2.hpp>

#include "vvipers/Collisions/CollidingSegment.hpp"

namespace VVipers {

class CollidingBody {
  public:
    virtual ~CollidingBody() {}
    virtual std::vector<std::shared_ptr<const CollidingSegment>> colliding_segments()
        const = 0;
    bool operator==(const CollidingBody& other) const{
        return this == &other;
    }
};

}  // Namespace VVipers

#endif  // VVIPERS_COLLISIONS_COLLIDING_BODY_HPP