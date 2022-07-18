#ifndef VVIPERS_COLLIDABLE_HPP
#define VVIPERS_COLLIDABLE_HPP

#include <vector>

namespace VVipers{

class CollisionBody;

class Collidable {
  public:
    virtual std::vector<const CollisionBody*> collisionBodies() const = 0;
};

}

#endif // VVIPERS_COLLIDABLE_HPP
