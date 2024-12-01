#include <vvipers/GameElements/Controller.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/GameElements/GameEvent.hpp"

namespace VVipers {

void KeyboardController::update(const Time&) {
    SteeringCommand cmd;
    // Only send event if something has changed since last time
    bool key_pressed;

    static bool last_left = false;
    key_pressed = sf::Keyboard::isKeyPressed(_keys.left);
    if (key_pressed) {
        // This is not the actual turn, but how much the viper wants to turn
        cmd.turn -= 1.;
        if (!last_left)
            cmd.enable = true;
    } else if (last_left) {
        cmd.enable = true;
    }
    last_left = key_pressed;

    static bool last_right = false;
    key_pressed = sf::Keyboard::isKeyPressed(_keys.right);
    if (key_pressed) {
        // This is not the actual turn, but how much the viper wants to turn
        cmd.turn += 1.;
        if (!last_right)
            cmd.enable = true;
    } else if (last_right) {
        cmd.enable = true;
    }
    last_right = key_pressed;

    static bool last_boost = false;
    key_pressed = sf::Keyboard::isKeyPressed(_keys.boost);
    if (key_pressed) {
        cmd.boost = true;
        if (!last_boost)
            cmd.enable = true;
    } else if (last_boost) {
        cmd.enable = true;
    }
    last_boost = key_pressed;

    set_steering_command(cmd);
    notify(ObjectModifiedEvent(this));
}

MouseController::MouseController(GameResources& game_resources)
    : _game_resources(game_resources) {
    _game_resources.window_manager().set_grab_mouse(true);
    _game_resources.window_manager().center_mouse();
}

void MouseController::update(const Time&) {
    const int window_half_width =
        _game_resources.window_manager().window_size().x / 2;
    const double full_turn_in_pixels = 1;  // Essentially the mouse sensitivity
    SteeringCommand command;
    command.turn = (_game_resources.window_manager().mouse_position().x -
                    window_half_width) / full_turn_in_pixels;
    command.boost = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    // This might cause problems if anything else uses the mouse
    _game_resources.window_manager().center_mouse();
    command.enable = true;
    set_steering_command(command);
    notify(ObjectModifiedEvent(this));
}

}  // namespace VVipers