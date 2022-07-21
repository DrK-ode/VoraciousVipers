#ifndef VVIPERS_GAMEEVENT_HPP
#define VVIPERS_GAMEEVENT_HPP

#include <SFML/Window/Event.hpp>
#include <vvipers/Collidable.hpp>
#include <vvipers/GameObject.hpp>
#include <vvipers/Time.hpp>

namespace VVipers {

class Bodypart;
class CollisionBody;
class Controller;

class GameEvent {
  public:
    enum class EventType {
        Application,
        Collision,
        Destroy,
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
    CollisionEvent(const Colliders& c)
        : GameEvent(EventType::Collision), colliders(c) {}
    GameEvent* clone() const override { return new CollisionEvent(*this); }
    const Colliders colliders;
};

class DestroyEvent : public GameEvent {
  public:
    DestroyEvent(const GameObject* optr)
        : GameEvent(EventType::Destroy), objectPtr(optr) {}
    GameEvent* clone() const override { return new DestroyEvent(*this); }
    const GameObject* objectPtr;
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
    SteeringEvent(const Controller* c)
        : GameEvent(EventType::Steering), controller(c), deltaAngle(0), boost(false) {}
    GameEvent* clone() const override { return new SteeringEvent(*this); }
    const Controller* controller;
    double deltaAngle;
    bool boost;
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

#endif // VVIPERS_GAMEEVENT_HPP
