#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

// Forward declaration in order to avoid circular dependencies
class Object;
class GameObject;
class MenuItem;
class Scene;

class GameEvent {
  public:
    enum class EventType {
        Destroy,
        Keyboard,
        Mouse,
        Scoring,
        Scene,
        ObjectModified,
        Window
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
        : GameEvent(EventType::Destroy), object_pointer(optr) {}
    GameEvent* clone() const override { return new DestroyEvent(*this); }
    const GameObject* object_pointer;
};

class KeyboardEvent : public GameEvent {
  public:
    enum class KeyboardEventType { KeyPressed, KeyReleased };
    KeyboardEvent(KeyboardEventType type, sf::Keyboard::Scancode code)
        : GameEvent(EventType::Keyboard),
          keyboard_event_type(type),
          scancode(code) {}
    GameEvent* clone() const override { return new KeyboardEvent(*this); }
    const KeyboardEventType keyboard_event_type;
    const sf::Keyboard::Scancode scancode;
};

class MouseEvent : public GameEvent {
  public:
    enum class MouseEventType { ButtonPressed, ButtonReleased, Move, Scroll };
    MouseEvent(MouseEventType type, const sf::Vector2i& pos)
        : GameEvent(EventType::Mouse), mouse_event_type(type), position(pos) {}
    GameEvent* clone() const override { return new MouseEvent(*this); }
    MouseEventType mouse_event_type;
    union {
        int mouse_button;
        double scroll;
    };
    const sf::Vector2i position;
};

class ObjectModifiedEvent : public GameEvent {
  public:
    ObjectModifiedEvent(const Object* object_ptr)
        : GameEvent(EventType::ObjectModified), object_pointer(object_ptr) {}
    GameEvent* clone() const override { return new ObjectModifiedEvent(*this); }
    const Object* object_pointer;
};

class SceneEvent : public GameEvent {
  public:
    enum class SceneEventType {
        Clear,     // Pop the whole stack and push the new Scene.
        Default,   // Pop the whole stack and push the default Scene.
        JumpTo,    // Pop until the chosen Scene is reached.
        Replace,   // Pop this Scene and push the new Scene.
        Return,    // Pop this Scene.
        Spawn,     // Push the new Scene.
        Quit       // Pop all Scenes.
    };
    SceneEvent(SceneEventType type)
        : GameEvent(EventType::Scene), scene_event_type(type) {}
    GameEvent* clone() const override { return new SceneEvent(*this); }
    const SceneEventType scene_event_type;
    std::shared_ptr<Scene> target_scene;
};

class ScoringEvent : public GameEvent {
  public:
    ScoringEvent(uint64_t s) : GameEvent(EventType::Scoring), score(s) {}
    GameEvent* clone() const override { return new ScoringEvent(*this); }
    const uint64_t score;
};

class WindowEvent : public GameEvent {
  public:
    enum class WindowEventType { WindowClosed, WindowResized };
    WindowEvent(const WindowEventType type)
        : GameEvent(EventType::Window), window_event_type(type) {}
    GameEvent* clone() const override { return new WindowEvent(*this); }
    const WindowEventType window_event_type;
};

}  // namespace VVipers
