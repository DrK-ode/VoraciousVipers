#include <vvipers/UIElements/MenuItem.hpp>

namespace VVipers {

sf::FloatRect MenuItem::getLocalBounds() const {
    return sf::FloatRect(m_position.x, m_position.y, m_size.x, m_size.y);
}

void MenuItem::setSize(Vec2 size) {
    m_size = size;
    on_geometry_change();
}

void MenuItem::setPosition(Vec2 position) {
    m_position = position;
    on_geometry_change();
}

void MenuItem::setSelected(bool selected) {
    m_selected = selected;
    on_selection();
}

}  // namespace VVipers
