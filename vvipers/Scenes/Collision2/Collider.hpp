#ifndef VVIPERS_SCENES_COLLISION2_COLLIDER_HPP
#define VVIPERS_SCENES_COLLISION2_COLLIDER_HPP

#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

class Collider;
class ColliderCircle;
class ColliderPolygon;
class ColliderSegmented;

struct ColliderPtr {
    const Collider* collider;            // Colliding object/segment
    std::unique_ptr<ColliderPtr> child;  // Colliding segment
    explicit operator bool() const { return collider; }
};

struct CollisionResult {
    std::unique_ptr<ColliderPtr> colliderA;
    std::unique_ptr<ColliderPtr> colliderB;
    std::unique_ptr<CollisionResult> nextCollision;

    explicit operator bool() const { return colliderA and colliderB; }
};

class Collider {
  public:
    enum class ColliderType { CircleLike, Polygon, Segmented };

    Collider(ColliderType type, bool active) : m_type(type), m_active(active) {}
    ColliderType getType() const { return m_type; }
    virtual bool inside(Vec2) const = 0;
    virtual sf::FloatRect getBounds() const = 0;
    std::unique_ptr<CollisionResult> collision(const Collider& body) const;
    void setActive(bool active) { m_active = active; }
    bool isActive() const { return m_active; }

  private:
    std::unique_ptr<CollisionResult> collisionSegmented(
        const ColliderSegmented& body) const;
    virtual std::unique_ptr<CollisionResult> collisionCircle(
        const ColliderCircle& circle) const = 0;
    virtual std::unique_ptr<CollisionResult> collisionPolygon(
        const ColliderPolygon& polygon) const = 0;

    ColliderType m_type;
    bool m_active;
};

/** Wraps any object that provides the methods getPosition() and getRadius().
 * Takes shared ownership of the wrapped object **/
class ColliderCircle : public Collider {
  private:
    struct CircleLikeObject {
        virtual double getRadius() const = 0;
        virtual Vec2 getPosition() const = 0;
    };

    template <typename T>
    class CircleWrapper : public CircleLikeObject {
      public:
        CircleWrapper(const T& circleObject)
            : wrappedCircleObject(circleObject) {}
        Vec2 getPosition() const override {
            return wrappedCircleObject.getPosition();
        }
        double getRadius() const override {
            return wrappedCircleObject.getRadius();
        }
        const T wrappedCircleObject;
    };

  public:
    template <typename T>
    ColliderCircle(const T& object, bool active)
        : Collider(ColliderType::CircleLike, active),
          m_circleLikeObject( std::make_unique<CircleWrapper<T>>(object)) {}
    Vec2 getPosition() const { return m_circleLikeObject->getPosition(); }
    double getRadius() const { return m_circleLikeObject->getRadius(); }

    /** Overrides from Collider **/
    sf::FloatRect getBounds() const override;
    bool inside(Vec2) const override;

  private:
    /** Overrides from Collider **/
    std::unique_ptr<CollisionResult> collisionCircle(
        const ColliderCircle& circle) const override;
    std::unique_ptr<CollisionResult> collisionPolygon(
        const ColliderPolygon& polygon) const override;

    const std::unique_ptr<CircleLikeObject> m_circleLikeObject;
};

/** Wraps any object that provides the methods getPointCount() and
 * getPoint(size_t). Takes shared ownership of the wrapped object **/
class ColliderPolygon : public Collider {
  private:
    struct PolygonObject {
        virtual size_t getPointCount() const = 0;
        virtual Vec2 getGlobalPoint(size_t) const = 0;
    };

    template <typename T>
    class PolygonWrapper : public PolygonObject {
      public:
        PolygonWrapper(const T& polygonObject)
            : wrappedPolygonObject(polygonObject) {}
        Vec2 getGlobalPoint(size_t i) const override {
            return wrappedPolygonObject.getGlobalPoint(i);
        }
        size_t getPointCount() const override {
            return wrappedPolygonObject.getPointCount();
        }

      private:
        const T wrappedPolygonObject;
    };

  public:
    template <typename T>
    ColliderPolygon(const T& object, bool active)
        : Collider(ColliderType::Polygon, active),
          m_polygonObject( std::make_unique<PolygonWrapper<T>>(object)) {}
    Vec2 getGlobalPoint(size_t i) const {
        return m_polygonObject->getGlobalPoint(i);
    }
    size_t getPointCount() const { return m_polygonObject->getPointCount(); }

    /** Overrides from Collider **/
    sf::FloatRect getBounds() const override;
    bool inside(Vec2) const override;

  private:
    /** Overrides from Collider **/
    std::unique_ptr<CollisionResult> collisionCircle(
        const ColliderCircle& circle) const override;
    std::unique_ptr<CollisionResult> collisionPolygon(
        const ColliderPolygon& polygon) const override;

    const std::unique_ptr<PolygonObject> m_polygonObject;
};

/** Wraps any object that provides the methods getSegmentCount() and
 * getSegment(size_t). Takes shared ownership of the wrapped object **/
class ColliderSegmented : public Collider {
  private:
    struct SegmentedObject {
        virtual size_t getSegmentCount() const = 0;
        virtual size_t getSegmentPointCount(size_t) const = 0;
        virtual Vec2 getSegmentGlobalPoint(size_t, size_t) const = 0;
        virtual bool isSegmentActive(size_t i) const = 0;
    };

    template <typename T>
    class SegmentedWrapper : public SegmentedObject {
      public:
        SegmentedWrapper(const T& polygonObject)
            : wrappedSegmentedObject(polygonObject) {}
        size_t getSegmentCount() const override {
            return wrappedSegmentedObject.getSegmentCount();
        }
        Vec2 getSegmentGlobalPoint(size_t i, size_t j) const override {
            return wrappedSegmentedObject.getSegmentGlobalPoint(i, j);
        }
        size_t getSegmentPointCount(size_t i) const override {
            return wrappedSegmentedObject.getSegmentPointCount(i);
        }
        bool isSegmentActive(size_t i) const {
            return wrappedSegmentedObject.isSegmentActive(i);
        }

      private:
        const T wrappedSegmentedObject;
    };

  public:
    class PolygonSegment {
      public:
        PolygonSegment(const ColliderSegmented& segmented)
            : m_segmented(segmented) {}
        /** These four methods allow the PolygonSegment to act as a base for a
         * ColliderPolygon but getting its data from a ColliderSegmented. Which
         * of the segments that are active is controlled by the
         * setCurrentSegment method. **/
        bool isActive() const {
            return m_segmented.isSegmentActive(m_currentSegment);
        }
        void setCurrentSegment(size_t i) { m_currentSegment = i; }
        Vec2 getGlobalPoint(size_t j) const {
            return m_segmented.getSegmentGlobalPoint(m_currentSegment, j);
        }
        size_t getPointCount() const {
            return m_segmented.getSegmentPointCount(m_currentSegment);
        }

      private:
        const ColliderSegmented& m_segmented;
        size_t m_currentSegment;
    };

  public:
    template <typename T>
    ColliderSegmented(const T& object, bool active)
        : Collider(ColliderType::Segmented, active),
          m_segmentedObject( std::make_unique<SegmentedWrapper<T>>(object)) {}
    size_t getSegmentCount() const {
        return m_segmentedObject->getSegmentCount();
    }
    Vec2 getSegmentGlobalPoint(size_t i, size_t j) const {
        return m_segmentedObject->getSegmentGlobalPoint(i, j);
    }
    size_t getSegmentPointCount(size_t i) const {
        return m_segmentedObject->getSegmentPointCount(i);
    }
    bool isSegmentActive(size_t i) const { return m_segmentedObject->isSegmentActive(i); }

    /** Overrides from Collider **/
    sf::FloatRect getBounds() const override;
    bool inside(Vec2) const override;

  private:
    /** Overrides from Collider **/
    std::unique_ptr<CollisionResult> collisionCircle(
        const ColliderCircle& circle) const override;
    std::unique_ptr<CollisionResult> collisionPolygon(
        const ColliderPolygon& polygon) const override;

    const std::unique_ptr<SegmentedObject> m_segmentedObject;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_COLLISION2_COLLIDER_HPP