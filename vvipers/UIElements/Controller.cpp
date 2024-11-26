#include <vvipers/UIElements/Controller.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

SteeringCommand KeyboardController::control() const {
    SteeringCommand cmd;
    // Only send event if something has changed since last time
    bool key_pressed;
    cmd.enable = false;

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

    return cmd;
}

MouseController::MouseController(Game& game) : _game(game) {
    _game.set_grab_mouse(true);
    sf::Mouse::setPosition(sf::Vector2i(0.5 * _game.window().getSize()),
                           _game.window());
}

SteeringCommand MouseController::control() const {
    auto& window = _game.window();
    const sf::Vector2i window_half_size(window.getSize().x / 2,
                                        window.getSize().y / 2);
    const double degree_per_pixel = 1;  // Essentially the mouse sensitivity
    SteeringCommand command;
    command.turn = degree_per_pixel *
                   (sf::Mouse::getPosition(window).x - window_half_size.x);
    command.boost = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    // This might cause problems if anything else uses the mouse
    sf::Mouse::setPosition(window_half_size, window);
    return command;
}

}  // namespace VVipers