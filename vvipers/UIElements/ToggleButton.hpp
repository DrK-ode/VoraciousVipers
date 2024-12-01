#pragma once

#include "vvipers/UIElements/MenuButton.hpp"

namespace VVipers {

class ToggleButton : public MenuButton {
  public:
    ToggleButton(const std::string& on_label,
                 const std::string& off_label, bool initial_state);
    bool is_toggled() const { return _toggle_state; }
    void set_toggle_state(bool state) {
        if (_toggle_state != state) {
            toggle();
        }
    }
    void toggle();

  private:
    void update_label();
    const std::string _on_label;
    const std::string _off_label;
    bool _toggle_state;
};
}  // namespace VVipers