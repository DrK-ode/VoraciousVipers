#include "vvipers/UIElements/ToggleButton.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "vvipers/UIElements/MenuButton.hpp"

namespace VVipers {

ToggleButton::ToggleButton(const std::string& enabled_label,
                           const std::string& disabled_label,
                           bool initial_state)
    : MenuButton(),
      _enabled_label(enabled_label),
      _disabled_label(disabled_label),
      _toggle_state(initial_state) {
    update_label();
}

void ToggleButton::update_label() {
    if (_toggle_state) {
        set_label(_enabled_label);
    } else {
        set_label(_disabled_label);
    }
}

}  // namespace VVipers