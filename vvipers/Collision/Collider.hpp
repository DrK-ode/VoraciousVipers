#ifndef VVIPERS_COLLISION_COLLIDER_HPP
#define VVIPERS_COLLISION_COLLIDER_HPP

#include <SFML/Graphics/Rect.hpp>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

class Collider;
class ColliderCircle;
class ColliderPolygon;
class ColliderSegmented;

struct ColliderSegment{
    const Collider* collider = nullptr;
    size_t segmentIndex = 0;
    explicit operator bool() const {return collider;}
};
struct CollisionResult{
    ColliderSegment colliderA;
    ColliderSegment colliderB;
    explicit operator bool() const { return colliderA and colliderB;}
};
using CollisionVector = std::vector< CollisionResult >;

class Collider {
  public:
    enum class ColliderType { CircleLike, Polygon, Segmented };

    Collider(ColliderType type, bool active) : m_type(type), m_active(active) {}
    virtual ~Collider() {}
    ColliderType getType() const { return m_type; }
    virtual bool inside(Vec2) const = 0;
    virtual sf::FloatRect getBounds() const = 0;
    void setActive(bool active) { m_active = active; }
    bool isActive() const { return m_active; }

    static CollisionVector collision(const Collider& body1, const Collider& body2);

  private:
    static CollisionResult collisionCirclePolygon(
        const ColliderCircle& circle1, const ColliderPolygon& polygon2);
    // Special function for the case of two segmented colliders
    static CollisionVector collisionBothSegmented(
        const ColliderSegmented& segmented1,
        const ColliderSegmented& segmented2);
    // Not for the vcase of two segmented colliders
    static CollisionVector collisionSegmented(
        const ColliderSegmented& segmented1, const Collider& body2);
    static CollisionResult collisionCircleCircle(
        const ColliderCircle& circle1, const ColliderCircle& circle2);
    static CollisionResult collisionPolygonPolygon(
        const ColliderPolygon& polygon1, const ColliderPolygon& polygon2);

    ColliderType m_type;
    bool m_active;
};

/** Wraps any object that provides the methods getPosition() and getRadius().
 * Takes shared ownership of the wrapped object **/
class ColliderCircle : public Collider {
  public:
    ColliderCircle(bool active = true)
        : Collider(ColliderType::CircleLike, active) {}
    virtual double getRadius() const = 0;
    virtual Vec2 getPosition() const = 0;

    /** Overrides from Collider **/
    virtual sf::FloatRect getBounds() const override;
    virtual bool inside(Vec2) const override;
};

/** Wraps any object that provides the methods getPointCount() and
 * getPoint(size_t). Takes shared ownership of the wrapped object **/
class ColliderPolygon : public Collider {
  public:
    ColliderPolygon(bool active = true)
        : Collider(ColliderType::Polygon, active) {}
    virtual size_t getPointCount() const = 0;
    virtual Vec2 getGlobalPoint(size_t) const = 0;

    /** Overrides from Collider **/
    virtual sf::FloatRect getBounds() const override;
    virtual bool inside(Vec2) const override;
};

/** Wraps any object that provides the methods getSegmentCount() and
 * getSegment(size_t). Takes shared ownership of the wrapped object **/
class ColliderSegmented : public Collider {
  public:
    class PolygonSegment : public ColliderPolygon {
      public:
        PolygonSegment(const ColliderSegmented& segmented, bool active = false)
            : ColliderPolygon(active), m_segmented(segmented) {}
        /** These four methods allow the PolygonSegment to act as a base for a
         * ColliderPolygon but getting its data from a ColliderSegmented. Which
         * of the segments that are active is controlled by the
         * setCurrentSegment method. **/
        bool isActive() const {
            return m_segmented.isSegmentActive(m_currentSegment);
        }
        void setCurrentSegment(size_t i) { m_currentSegment = i; }
        Vec2 getGlobalPoint(size_t j) const override {
            return m_segmented.getSegmentGlobalPoint(m_currentSegment, j);
        }
        size_t getPointCount() const override {
            return m_segmented.getSegmentPointCount(m_currentSegment);
        }

      private:
        const ColliderSegmented& m_segmented;
        size_t m_currentSegment;
    };

    ColliderSegmented(bool active = true)
        : Collider(ColliderType::Segmented, active) {}
    virtual size_t getSegmentCount() const = 0;
    virtual size_t getSegmentPointCount(size_t) const = 0;
    virtual Vec2 getSegmentGlobalPoint(size_t, size_t) const = 0;
    virtual bool isSegmentActive(size_t i) const = 0;

    /** Overrides from Collider **/
    virtual sf::FloatRect getBounds() const override;
    virtual bool inside(Vec2) const override;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISION_COLLIDER_HPP