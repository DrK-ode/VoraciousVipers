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
        MouseMove,
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

class DestroyEvent : public GameEvent {
  public:
    DestroyEvent(const GameObject* optr)
        : GameEvent(EventType::Destroy), objectPtr(optr) {}
    GameEvent* clone() const override { return new DestroyEvent(*this); }
    const GameObject* objectPtr;
};

class UpdateEvent : public GameEvent {
  public:
    UpdateEvent(const Time& time)
        : GameEvent(EventType::Update), elapsedTime(time) {}
    GameEvent* clone() const override { return new UpdateEvent(*this); }
    const Time elapsedTime;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEEVENT_HPP
