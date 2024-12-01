#include "vvipers/UIElements/ToggleButton.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "vvipers/GameElements/GameEvent.hpp"
#include "vvipers/UIElements/MenuButton.hpp"

namespace VVipers {

ToggleButton::ToggleButton(const std::string& enabled_label,
                           const std::string& disabled_label,
                           bool initial_state)
    : MenuButton(),
      _on_label(enabled_label),
      _off_label(disabled_label),
      _toggle_state(initial_state) {
    update_label();
}

void ToggleButton::toggle() {
    _toggle_state = !_toggle_state;
    update_label();
    notify(ObjectModifiedEvent(this));
}

void ToggleButton::update_label() {
    if (_toggle_state) {
        set_label(_on_label);
    } else {
        set_label(_off_label);
    }
}

}  // namespace VVipers