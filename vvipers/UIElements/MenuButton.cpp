#include <vvipers/UIElements/MenuButton.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

MenuButton::MenuButton() {
    _text.setFillColor(sf::Color::Magenta);
    _box.setFillColor(sf::Color::Transparent);
    _box.setOutlineColor(sf::Color::Transparent);
}

void MenuButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_box, states);
    target.draw(_text, states);
}

void MenuButton::on_selection() {
    if (is_selected())
        _box.setOutlineThickness(-std::max(1., 0.05 * size().y));
    else
        _box.setOutlineThickness(0);
}

void MenuButton::on_geometry_change() {
    Vec2 size = this->size();
    Vec2 position = this->position();
    if (size == Vec2(0, 0))
        return;

    _box.setPosition(position);
    _box.setSize(size);

    /*unsigned int font_size = 50;
    auto margin = 0.2 * size.y;
    while(font_size > 10){
        _text.setCharacterSize(font_size);
        auto bounds = _text.getLocalBounds();
        if( bounds.height + margin < size.y && bounds.width + margin < size.x){
            break;
        }
        --font_size;
        }*/
    _text.setCharacterSize(60);
    auto bounds = _text.getLocalBounds();
    _text.setOrigin(bounds.left + 0.5 * bounds.width,
                    bounds.top + 0.5 * bounds.height);
    _text.setPosition(position + size / 2);

    on_selection();
}

void MenuButton::set_colors(sf::Color fill, sf::Color border, sf::Color text) {
    _box.setFillColor(fill);
    _box.setOutlineColor(border);
    _text.setFillColor(text);
    _text.setOutlineColor(border);
}

void MenuButton::set_font(const sf::Font& font) { _text.setFont(font); }

void MenuButton::set_label(const std::string& label) { _text.setString(label); }

}  // namespace VVipers
