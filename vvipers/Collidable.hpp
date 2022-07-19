#ifndef VVIPERS_COLLIDABLE_HPP
#define VVIPERS_COLLIDABLE_HPP

#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>
#include <ostream>

namespace VVipers {

class CollisionBody;
class Collidable;
class Bodypart;

struct CollisionTriplet {
    const Collidable* collidable;
    const CollisionBody* collisionBody;
    const Bodypart* bodypart;

    bool operator==(const CollisionTriplet& right) const {
        return this->collidable == right.collidable &&
               this->collisionBody == right.collisionBody &&
               this->bodypart == right.bodypart;
    }
    bool operator!=(const CollisionTriplet& right) const {
        return !(this->operator==(right));
    }
};
typedef std::pair<CollisionTriplet, CollisionTriplet> Colliders;

std::ostream& operator<<(std::ostream& os, const CollisionTriplet& ct );
std::ostream& operator<<(std::ostream& os, const Colliders& c );

class Collidable {
  public:
    Collidable(const std::string& id) : CID(id) {}
    virtual std::vector<const CollisionBody*> collisionBodies() const = 0;
    virtual sf::Rect<double> rectangularBounds() const;

    static std::vector<Colliders> collision(const Collidable* coll1,
                                            const Collidable* coll2);

    const std::string CID;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLIDABLE_HPP
