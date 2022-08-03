#include <vvipers/Scenes/UIElements/MenuItem.hpp>

namespace VVipers {

sf::FloatRect MenuItem::getLocalBounds() const {
    return sf::FloatRect(m_position.x, m_position.y, m_size.x, m_size.y);
}

void MenuItem::setSize(Vec2 size) {
    m_size = size;
    onGeometryChange();
}

void MenuItem::setPosition(Vec2 position) {
    m_position = position;
    onGeometryChange();
}

}  // namespace VVipers
