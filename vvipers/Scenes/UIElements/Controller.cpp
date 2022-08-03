#include <vvipers/Scenes/UIElements/Controller.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

SteeringCommand KeyboardController::control() const {
    SteeringCommand cmd;
    // Only send event if something has changed since last time
    bool keyPressed;
    cmd.enable = false;

    static bool lastLeft = false;
    keyPressed = sf::Keyboard::isKeyPressed(m_keys.left);
    if (keyPressed) {
        cmd.turn -= 1.0;  // Hard turn left
        if (!lastLeft)
            cmd.enable = true;
    } else if (lastLeft) {
        cmd.enable = true;
    }
    lastLeft = keyPressed;

    static bool lastRight = false;
    keyPressed = sf::Keyboard::isKeyPressed(m_keys.right);
    if (keyPressed) {
        cmd.turn += 1.;  // Hard turn right
        if (!lastRight)
            cmd.enable = true;
    } else if (lastRight) {
        cmd.enable = true;
    }
    lastRight = keyPressed;

    static bool lastBoost = false;
    keyPressed = sf::Keyboard::isKeyPressed(m_keys.boost);
    if (keyPressed) {
        cmd.boost = true;
        if (!lastBoost)
            cmd.enable = true;
    } else if (lastBoost) {
        cmd.enable = true;
    }
    lastBoost = keyPressed;

    return cmd;
}

MouseController::MouseController(Game& game) : m_game(game) {
    m_game.setGrabMouse(true);
    sf::Mouse::setPosition(sf::Vector2i(0.5 * m_game.getWindow().getSize()),
                           m_game.getWindow());
}

SteeringCommand MouseController::control() const {
    auto& window = m_game.getWindow();
    const sf::Vector2i windowHalfSize(window.getSize().x / 2,
                                      window.getSize().y / 2);
    const double degPerPx = 1;  // Essentially the mouse sensitivity
    SteeringCommand cmd;
    cmd.turn = degPerPx * (sf::Mouse::getPosition(window).x - windowHalfSize.x);
    cmd.boost = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    // This might cause problems if anything else uses the mouse
    sf::Mouse::setPosition(windowHalfSize, window);
    return cmd;
}

}  // namespace VVipers