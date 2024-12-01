#include "vvipers/UIElements/SelectionButton.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <stdexcept>

#include "vvipers/GameElements/GameEvent.hpp"
#include "vvipers/UIElements/MenuButton.hpp"

namespace VVipers {

template <typename T>
SelectionButton<T>::SelectionButton(const std::string& pre_label,
                                    const std::vector<T>& options)
    : MenuButton(),
      _pre_label(pre_label),
      _options(options),
      _selected_option(0) {
    if (options.size() == 0) {
        throw std::runtime_error(
            "SelectionButton requires a minimum of one selectable option.");
    }
    update_label();
}

template <typename T>
void SelectionButton<T>::option_left() {
    set_selected_option(_selected_option - 1);
}

template <typename T>
void SelectionButton<T>::option_right() {
    set_selected_option(_selected_option + 1);
}

template <typename T>
void SelectionButton<T>::set_selected_option(size_t new_option) {
    if (new_option < 0 || new_option >= _options.size()) {
        return;
    }
    _selected_option = new_option;
    update_label();
    notify(ObjectModifiedEvent(this));
}

template <>
void SelectionButton<std::string>::update_label() {
    set_label(_pre_label + _options[_selected_option]);
}
template <typename T>
void SelectionButton<T>::update_label() {
    set_label(_pre_label + std::to_string(_options[_selected_option]));
}

template class SelectionButton<int>;
template class SelectionButton<size_t>;
template class SelectionButton<std::string>;

}  // namespace VVipers