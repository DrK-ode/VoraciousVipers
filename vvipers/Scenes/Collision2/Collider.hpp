#ifndef VVIPERS_SCENES_COLLISION2_COLLIDER_HPP
#define VVIPERS_SCENES_COLLISION2_COLLIDER_HPP

#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

class ColliderCircle;
class ColliderPolygon;
class ColliderSegmented;

class Collider {
  public:
    enum class ColliderType { CircleLike, Polygon, Segmented };

    Collider(ColliderType type);
    ColliderType getType() const { return m_type; }
    virtual bool inside(Vec2) const = 0;
    virtual sf::FloatRect getBounds() const = 0;
    virtual bool collision(const Collider& body) const;
    bool collisionSegmented(const ColliderSegmented& body) const;
    virtual bool collisionCircle(const ColliderCircle& circle) const = 0;
    virtual bool collisionPolygon(const ColliderPolygon& polygon) const = 0;

  private:
    ColliderType m_type;
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

      private:
        T wrappedCircleObject;
    };

  public:
    template <typename T>
    ColliderCircle(const T& object)
        : Collider(ColliderType::CircleLike),
          m_circleLikeObject(std::make_shared<CircleWrapper<T>>(object)) {}
    Vec2 getPosition() const { return m_circleLikeObject->getPosition(); }
    double getRadius() const { return m_circleLikeObject->getRadius(); }

    /** Overrides from Collider **/
    sf::FloatRect getBounds() const override;
    bool inside(Vec2) const override;
    bool collisionCircle(const ColliderCircle& circle) const override;
    bool collisionPolygon(const ColliderPolygon& polygon) const override;

  private:
    std::shared_ptr<CircleLikeObject> m_circleLikeObject;
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
            return wrappedPolygonObject.getPoint(i);
        }
        size_t getPointCount() const override {
            return wrappedPolygonObject.getPointCount();
        }

      private:
        T wrappedPolygonObject;
    };

  public:
    template <typename T>
    ColliderPolygon(const T& object)
        : Collider(ColliderType::Polygon),
          m_polygonObject(std::make_shared<PolygonWrapper<T>>(object)) {}
    Vec2 getGlobalPoint(size_t i) const {
        return m_polygonObject->getGlobalPoint(i);
    }
    size_t getPointCount() const { return m_polygonObject->getPointCount(); }

    /** Overrides from Collider **/
    sf::FloatRect getBounds() const override;
    bool inside(Vec2) const override;
    bool collisionCircle(const ColliderCircle& circle) const override;
    bool collisionPolygon(const ColliderPolygon& polygon) const override;

  private:
    std::shared_ptr<PolygonObject> m_polygonObject;
};

/** Wraps any object that provides the methods getSegmentCount() and
 * getSegment(size_t). Takes shared ownership of the wrapped object **/
class ColliderSegmented : public Collider {
  private:
    struct SegmentedObject {
        virtual size_t getSegmentCount() const = 0;
        virtual const Collider& getSegment(size_t) const = 0;
    };

    template <typename T>
    class SegmentedWrapper : public SegmentedObject {
      public:
        SegmentedWrapper(const T& polygonObject)
            : wrappedSegmentedObject(polygonObject) {}
        const Collider& getSegment(size_t i) const override {
            return wrappedSegmentedObject.getSegment(i);
        }
        size_t getSegmentCount() const override {
            return wrappedSegmentedObject.getSegmentCount();
        }

      private:
        T wrappedSegmentedObject;
    };

  public:
    template <typename T>
    ColliderSegmented(const T& object)
        : Collider(ColliderType::Segmented),
          m_segmentedObject(std::make_shared<SegmentedWrapper<T>>(object)) {}
    size_t getSegmentCount() const {
        return m_segmentedObject->getSegmentCount();
    }
    const Collider& getSegment(size_t i) const {
        return m_segmentedObject->getSegment(i);
    }

    /** Overrides from Collider **/
    sf::FloatRect getBounds() const override;
    bool inside(Vec2) const override;
    bool collisionCircle(const ColliderCircle& circle) const override;
    bool collisionPolygon(const ColliderPolygon& polygon) const override;

  private:
    std::shared_ptr<SegmentedObject> m_segmentedObject;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_COLLISION2_COLLIDER_HPP