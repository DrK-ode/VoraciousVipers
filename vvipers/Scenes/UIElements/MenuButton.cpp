#include <vvipers/Scenes/UIElements/MenuButton.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

MenuButton::MenuButton() : m_fontRatio(0.8) {
    m_text.setFillColor(sf::Color::Magenta);
    m_box.setFillColor(sf::Color::Transparent);
    m_box.setOutlineColor(sf::Color::Transparent);
}

void MenuButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_box, states);
    target.draw(m_text, states);
}

void MenuButton::update(Time elapsedTime) {
    if (isSelected())
        m_box.setOutlineColor( m_text.getFillColor() );
    else
        m_box.setOutlineColor(sf::Color::Transparent);
}

void MenuButton::onGeometryChange() {
    auto size = getSize();
    auto position = getPosition();

    m_text.setCharacterSize(m_fontRatio * size.y);
    auto textlb = m_text.getLocalBounds();
    m_text.setOrigin(textlb.left + 0.5 * textlb.width,
                     textlb.top + 0.5 * textlb.height);
    m_text.setPosition(position + size/2);

    m_box.setPosition(position);
    m_box.setSize(size);
    m_box.setOutlineThickness(-0.05 * size.y);
}

void MenuButton::setFont(const sf::Font& font) { m_text.setFont(font); }

void MenuButton::setLabel(const std::string& label) { m_text.setString(label); }

}  // namespace VVipers
