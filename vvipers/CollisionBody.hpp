#ifndef VVIPERS_COLLISIONBODY_HPP
#define VVIPERS_COLLISIONBODY_HPP

#include <vector>

namespace VVipers {

class Bodypart;

/** CollisionBody is built around a VertexArray but also divides the vertices
 * into BodyParts which must be convex in order for the collision detection to
 * work. The vertices can be set arbitrarily and the body parts are later
 * assigned by specifying which consequetive range of vertices are one body
 * part. No parts of a CollisionBody will ever be checked for collisions between
 * each other. **/

class CollisionBody {
  public:
    static std::vector<std::pair<const Bodypart*, const Bodypart*> > collision(
        const CollisionBody*, const CollisionBody*);

    virtual const std::vector<const Bodypart*> bodyParts() const = 0;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISIONBODY_HPP
