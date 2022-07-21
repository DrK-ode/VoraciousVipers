#ifndef VVIPERS_COLLISIONBODY_HPP
#define VVIPERS_COLLISIONBODY_HPP

#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>

namespace VVipers {

class Bodypart;

typedef uint64_t CBID_type;

/** A CollisionBody has a collection of bodyparts. No parts of a CollisionBody
 * will ever be checked for collisions between each other. **/
class CollisionBody {
  public:
    CollisionBody(CBID_type id) : CBID(id) {}

    static std::vector<std::pair<const Bodypart*, const Bodypart*> > collision(
        const CollisionBody*, const CollisionBody*);

    virtual const std::vector<const Bodypart*> bodyparts() const = 0;

    virtual sf::Rect<double> rectangularBounds() const;

    const CBID_type CBID;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISIONBODY_HPP
