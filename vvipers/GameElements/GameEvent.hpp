#ifndef VVIPERS_GAMEELEMENTS_GAMEEVENT_HPP
#define VVIPERS_GAMEELEMENTS_GAMEEVENT_HPP

#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Bodypart;
class CollisionBody;
class Controller;
class GameObject;
class MenuItem;
class Player;

class GameEvent {
  public:
    enum class EventType {
        Application,
        Destroy,
        Menu,
        Scoring,
        Update,
        ObjectModified
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

class MenuEvent : public GameEvent {
  public:
    enum class MenuEventType { Return, SubMenu };
    MenuEvent(const MenuItem* mi) : GameEvent(EventType::Menu), sender(mi) {}
    GameEvent* clone() const override { return new MenuEvent(*this); }
    const MenuItem* sender;
    MenuEventType message;
};

class ObjectModifiedEvent : public GameEvent {
  public:
    ObjectModifiedEvent(const GameObject* optr)
        : GameEvent(EventType::ObjectModified), objectPtr(optr) {}
    GameEvent* clone() const override { return new ObjectModifiedEvent(*this); }
    const GameObject* objectPtr;
};

class ScoringEvent : public GameEvent {
  public:
    ScoringEvent(const Player* p, uint64_t s)
        : GameEvent(EventType::Scoring), player(p), score(s) {}
    GameEvent* clone() const override { return new ScoringEvent(*this); }
    const Player* player;
    const uint64_t score;
};

class UpdateEvent : public GameEvent {
  public:
    UpdateEvent(const Time& time)
        : GameEvent(EventType::Update), elapsedTime(time) {}
    GameEvent* clone() const override { return new UpdateEvent(*this); }
    const Time elapsedTime;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEELEMENTS_GAMEEVENT_HPP
