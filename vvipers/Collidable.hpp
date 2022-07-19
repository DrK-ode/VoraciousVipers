#ifndef VVIPERS_COLLIDABLE_HPP
#define VVIPERS_COLLIDABLE_HPP

#include <vector>
#include <SFML/Graphics/Rect.hpp>

namespace VVipers{

class CollisionBody;
class Collidable;
class Bodypart;

typedef std::tuple<const Collidable*, const CollisionBody*, const Bodypart*> CollisionTuple;
typedef std::pair<CollisionTuple, CollisionTuple> Colliders;

class Collidable {
  public:
    virtual std::vector<const CollisionBody*> collisionBodies() const = 0;
    virtual sf::Rect<double> rectangularBounds() const;

    static std::vector<Colliders> collision(const Collidable* coll1, const Collidable* coll2);
};

}

#endif // VVIPERS_COLLIDABLE_HPP
