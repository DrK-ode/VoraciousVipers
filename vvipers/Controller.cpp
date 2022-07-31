#include <vvipers/Controller.hpp>
#include <vvipers/debug.hpp>

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

SteeringCommand MouseController::control() const {
    const sf::Vector2i windowHalfSize(m_window.getSize().x / 2,
                                      m_window.getSize().y / 2);
    const double degPerPx = 1;  // Essentially the mouse sensitivity
    SteeringCommand cmd;
    cmd.turn =
        degPerPx * (sf::Mouse::getPosition(m_window).x - windowHalfSize.x);
    cmd.boost = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    // This might cause problems if anything else uses the mouse
    sf::Mouse::setPosition(windowHalfSize, m_window);
    return cmd;
}

}  // namespace VVipers