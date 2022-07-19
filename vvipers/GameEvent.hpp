#ifndef VVIPERS_GAMEEVENT_HPP
#define VVIPERS_GAMEEVENT_HPP

#include <SFML/Window/Event.hpp>
#include <vvipers/Collidable.hpp>
#include <vvipers/Time.hpp>
#include <tuple>

namespace VVipers {

class Bodypart;
class CollisionBody;
class Controller;

class GameEvent {
  public:
    enum class EventType {
        Application,
        Collision,
        Destroyed,
        Keyboard,
        Mouse,
        Steering,
        Update,
        Window
    };
    virtual GameEvent* clone() const = 0;
    EventType type() const { return m_type; }
    virtual ~GameEvent(){};

  protected:
    GameEvent(EventType type) : m_type(type) {}

  private:
    EventType m_type;
};

class ApplicationEvent : public GameEvent {
  public:
    enum class ApplicationEventType { Exit };
    ApplicationEvent(ApplicationEventType type)
        : GameEvent(EventType::Application), eventType(type) {}
    GameEvent* clone() const override { return new ApplicationEvent(*this); }
    const ApplicationEventType eventType;
};

class CollisionEvent : public GameEvent {
  public:
    CollisionEvent( const Colliders& c ) : CollisionEvent( c.first, c.second) {}
    CollisionEvent( const CollisionTuple& a,
                   const CollisionTuple& b)
        : GameEvent(EventType::Collision),
          A(a),
          B(b) {}
    GameEvent* clone() const override { return new CollisionEvent(*this); }
    const CollisionTuple A;
    const CollisionTuple B;
};

class DestroyedEvent : public GameEvent {
  public:
    DestroyedEvent(const void* optr)
        : GameEvent(EventType::Destroyed), objectPtr(optr) {}
    GameEvent* clone() const override { return new DestroyedEvent(*this); }
    const void* objectPtr;
};

class KeyboardEvent : public GameEvent {
  public:
    KeyboardEvent(const sf::Event::KeyEvent& keyEvent)
        : GameEvent(EventType::Keyboard), keyInfo(keyEvent) {}
    GameEvent* clone() const override { return new KeyboardEvent(*this); }
    const sf::Event::KeyEvent keyInfo;
};

class SteeringEvent : public GameEvent {
  public:
    SteeringEvent(const Controller* c, double dA)
        : GameEvent(EventType::Steering), controller(c), deltaAngle(dA) {}
    GameEvent* clone() const override { return new SteeringEvent(*this); }
    const Controller* controller;
    const double deltaAngle;
};

class UpdateEvent : public GameEvent {
  public:
    UpdateEvent(const Time& time)
        : GameEvent(EventType::Update), elapsedTime(time) {}
    GameEvent* clone() const override { return new UpdateEvent(*this); }
    const Time elapsedTime;
};

class WindowEvent : public GameEvent {
  public:
    WindowEvent(sf::Event::EventType type)
        : GameEvent(EventType::Window), eventType(type) {}
    GameEvent* clone() const override { return new WindowEvent(*this); }
    const sf::Event::EventType eventType;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEEVENT_HPP
