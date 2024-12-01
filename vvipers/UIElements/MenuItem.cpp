#include <vvipers/UIElements/MenuItem.hpp>

namespace VVipers {

sf::FloatRect MenuItem::local_bounds() const {
    return sf::FloatRect(_position.x, _position.y, _size.x, _size.y);
}

void MenuItem::set_size(Vec2 size) {
    _size = size;
    on_geometry_change();
}

void MenuItem::set_position(Vec2 position) {
    _position = position;
    on_geometry_change();
}

void MenuItem::set_selected(bool selected) {
    _selected = selected;
    on_selection();
}

void MenuItem::enable(bool enable) {
    _enabled = enable;
    on_enable();
}

}  // namespace VVipers
