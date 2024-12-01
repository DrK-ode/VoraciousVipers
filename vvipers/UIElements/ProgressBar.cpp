#include <SFML/Graphics/RenderTarget.hpp>
#include <sstream>
#include <vvipers/UIElements/ProgressBar.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

ProgressBar::ProgressBar()
    : _vertical(false),
      _invert(false),
      _progress_low(0.),
      _progress_high(1.),
      _show_text(false),
      _text_style(ProgressTextStyle::Percent) {
    set_progress(_progress_low);
    set_size(_size);
    set_border_width(-1);
    _main_rectangle.setFillColor(sf::Color::Transparent);
    _main_rectangle.setOutlineColor(sf::Color::Transparent);
    _bar_rectangle.setFillColor(sf::Color::Transparent);
    _bar_rectangle.setOutlineColor(sf::Color::Transparent);
}

void ProgressBar::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(_bar_rectangle, states);
    target.draw(_main_rectangle, states);
    target.draw(_text, states);
}

void ProgressBar::set_position(Vec2 position) {
    _position = position;
    _main_rectangle.setPosition(position);
    update_bar();
}

void ProgressBar::set_border_width(double width) {
    _main_rectangle.setOutlineThickness(-width);
    _bar_rectangle.setOutlineThickness(-width);
}

void ProgressBar::set_progress_limits(double low, double high) {
    _progress_low = low;
    _progress_high = high;
}

void ProgressBar::set_progress(double progress) {
    _progress = progress;
    update_bar();
};

void ProgressBar::set_size(Vec2 size) {
    _size = size;
    _main_rectangle.setSize(size);
    update_bar();
}

void ProgressBar::set_text_properties(const sf::Font* font,
                                      double character_size, sf::Color color,
                                      ProgressTextStyle style) {
    _text.setFont(*font);
    _text.setCharacterSize(character_size);
    _text.setFillColor(color);
    _text_style = style;
}

void ProgressBar::set_vertical(bool vertical) {
    _vertical = vertical;
    update_bar();
}

void ProgressBar::set_inverted_bar(bool invert) {
    _invert = invert;
    update_bar();
}

void ProgressBar::update_bar() {
    double relativeProgress =
        std::max(0., std::min(1., (_progress - _progress_low) /
                                      (_progress_high - _progress_low)));
    double barStart = (_invert xor _vertical) ? 1. - relativeProgress : 0.;

    if (_vertical) {
        barStart *= _size.y;
        _bar_rectangle.setSize(
            sf::Vector2f(_size.x, _size.y * relativeProgress));
        _bar_rectangle.setPosition(_position.x, _position.y + barStart);
    } else {
        barStart *= _size.x;
        _bar_rectangle.setSize(
            sf::Vector2f(_size.x * relativeProgress, _size.y));
        _bar_rectangle.setPosition(_position.x + barStart, _position.y);
    }
    if (_show_text) {
        std::stringstream ss;
        switch (_text_style) {
            case ProgressTextStyle::IntegerRatio: {
                ss << int(_progress) << " / " << int(_progress_high);
                break;
            }
            case ProgressTextStyle::Percent: {
                ss << int(100 * (_progress - _progress_low) /
                          (_progress_high - _progress_low))
                   << '%';
                break;
            }
        }
        _text.setString(ss.str());
        auto lb = _text.getLocalBounds();
        _text.setOrigin(lb.left + 0.5 * lb.width, lb.top + 0.5 * lb.height);
        _text.setPosition(_position + 0.5 * _size);
    }
}

}  // namespace VVipers
