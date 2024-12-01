#include <vvipers/UIElements/MenuButton.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

MenuButton::MenuButton()
    : _fill_color(sf::Color::Red),
      _border_color(sf::Color::Green),
      _text_color(sf::Color::Blue) {
    update_colors();
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

    unsigned int font_size = 50;
    double margin = 0.06 * size.y;
    while (font_size > 10) {
        _text.setCharacterSize(font_size);
        auto bounds = _text.getLocalBounds();
        if (bounds.height + margin < size.y && bounds.width + margin < size.x) {
            break;
        }
        --font_size;
    }
    auto bounds = _text.getLocalBounds();
    _text.setOrigin(bounds.left + 0.5 * bounds.width,
                    bounds.top + 0.5 * bounds.height);
    _text.setPosition(position + size / 2);

    on_selection();
}

void MenuButton::on_enable() { 
    update_colors(); }

void MenuButton::set_colors(sf::Color fill, sf::Color border) {
    _fill_color = fill;
    _border_color = border;
    update_colors();
}

void MenuButton::set_text(const sf::Font& font, sf::Color text_color) {
    _text.setFont(font);
    _text_color = text_color;
    update_colors();
    on_geometry_change();
}

void MenuButton::set_label(const std::string& label) { _text.setString(label); }

void MenuButton::update_colors() {
    if (is_enabled()) {
        _text.setFillColor(_text_color);
        _box.setFillColor(_fill_color);
        _box.setOutlineColor(_border_color);
    } else {
        sf::Color color;
        color = _fill_color;
        color.a = 0.5 * color.a;
        _box.setFillColor(color);
        color = _text_color;
        color.a = 0.5 * color.a;
        _text.setFillColor(color);
        _box.setOutlineColor(_border_color);
    }
}

}  // namespace VVipers
