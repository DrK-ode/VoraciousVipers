#pragma once

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
    EventType type() const { return _type; }
    virtual ~GameEvent() {};

  protected:
    GameEvent(EventType type) : _type(type) {}

  private:
    EventType _type;
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
        : GameEvent(EventType::ObjectModified), object_pointer(optr) {}
    GameEvent* clone() const override { return new ObjectModifiedEvent(*this); }
    const GameObject* object_pointer;
};

class ScoringEvent : public GameEvent {
  public:
    ScoringEvent(uint64_t s) : GameEvent(EventType::Scoring), score(s) {}
    GameEvent* clone() const override { return new ScoringEvent(*this); }
    const uint64_t score;
};

}  // namespace VVipers
