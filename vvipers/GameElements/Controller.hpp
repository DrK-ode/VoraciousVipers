#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vvipers/Engine/GameResources.hpp>
#include <vvipers/Utilities/Time.hpp>

#include "vvipers/GameElements/GameObject.hpp"
#include "vvipers/GameElements/Observer.hpp"

namespace VVipers {

struct SteeringCommand {
    bool enable = false;
    double turn = 0.;
    bool boost = false;
};

class Controller : public Object, public Observable, public Observer {
  public:
    Controller() : _command() {}
    virtual ~Controller() {}
    void on_notify(const GameEvent&) override;
    SteeringCommand steering_command() const { return _command; }
    void set_steering_command(const SteeringCommand& command) {
        _command = command;
    }
    virtual void update(const Time&) {};

  private:
    SteeringCommand _command;
};

class KeyboardController : public Controller {
  public:
    struct KeyboardControls {
        sf::Keyboard::Scancode left;
        sf::Keyboard::Scancode right;
        sf::Keyboard::Scancode boost;
    };
    KeyboardController(const KeyboardControls& keys) : _keys(keys) {}

  private:
    void update(const Time&) override;
    KeyboardControls _keys;
};

class MouseController : public Controller {
  public:
    MouseController(GameResources&);
    ~MouseController() { _game_resources.window_manager()->set_grab_mouse(false); }

  private:
    void update(const Time&) override;
    GameResources& _game_resources;
};

}  // namespace VVipers
