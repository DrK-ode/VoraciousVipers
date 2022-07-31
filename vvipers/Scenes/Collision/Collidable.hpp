#ifndef VVIPERS_COLLIDABLE_HPP
#define VVIPERS_COLLIDABLE_HPP

#include <SFML/Graphics/Rect.hpp>
#include <ostream>
#include <string>
#include <vector>
#include <vvipers/Scenes/Collision/CollisionBody.hpp>

namespace VVipers {

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

class Collidable {
  public:
    virtual ~Collidable() {};
    virtual std::vector<const CollisionBody*> collisionBodies() const = 0;
    virtual sf::Rect<double> rectangularBounds() const;

    static std::vector<Colliders> collision(const Collidable* coll1,
                                            const Collidable* coll2);
};

/** Collidable wrapper around a CollisionBody **/
class MonoBodyCollidable : public Collidable {
  public:
    MonoBodyCollidable(CollisionBody* cb) { m_bodies.push_back(m_body = cb); }
    ~MonoBodyCollidable() { delete m_body;}
    CollisionBody* body() const { return m_body; }
    void body(CollisionBody* cb) { m_bodies[0] = m_body = cb; }
    std::vector<const CollisionBody*> collisionBodies() const override {
        return m_bodies;
    }

  private:
    CollisionBody* m_body;
    std::vector<const CollisionBody*> m_bodies;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLIDABLE_HPP
