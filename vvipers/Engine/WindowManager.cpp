#include "vvipers/Engine/WindowManager.hpp"
#include <SFML/System/Vector2.hpp>

namespace VVipers {

WindowManager::WindowManager(const sf::Vector2u& window_size) {
    _window.create(sf::VideoMode(window_size.x, window_size.y),
                   "VoraciousVipers");
    set_grab_mouse(false);
}

void WindowManager::center_mouse() {
    sf::Mouse::setPosition(sf::Vector2i(0.5 * window_size()), _window);
}

sf::Vector2i WindowManager::mouse_position() const{
    return sf::Mouse::getPosition(_window);
}

void WindowManager::set_grab_mouse(bool grabbed) {
    _is_mouse_grabbed = grabbed;
    _window.setMouseCursorGrabbed(grabbed);
    _window.setMouseCursorVisible(!grabbed);
}

}  // namespace VVipers