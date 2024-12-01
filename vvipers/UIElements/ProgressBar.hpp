#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

class ProgressBar : public sf::Drawable {
  public:
    enum class ProgressTextStyle { Percent, IntegerRatio };

    ProgressBar();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Vec2 position() const { return _position; }
    sf::FloatRect local_bounds() const {
        return _main_rectangle.getLocalBounds();
    }
    double progress() const { return _progress; }
    std::pair<double, double> progress_limits() const {
        return {_progress_low, _progress_high};
    }

    void set_border_color(sf::Color color) {
        _main_rectangle.setOutlineColor(color);
    }
    void set_background_color(sf::Color color) {
        _main_rectangle.setFillColor(color);
    }
    void set_bar_color(sf::Color color) { _bar_rectangle.setFillColor(color); }
    void set_border_width(double width);
    void set_progress_limits(double low, double high);
    void set_show_text(bool show) { _show_text = show; }
    void set_size(Vec2 size);
    void set_text_properties(const sf::Font* font, double sizeRatio,
                             sf::Color color, ProgressTextStyle style);
    void set_position(Vec2 pos);
    void set_progress(double progress);
    void set_vertical(bool vertical);
    void set_inverted_bar(bool invert);

  private:
    void update_bar();

    bool _vertical;
    bool _invert;
    Vec2 _size;
    Vec2 _position;
    double _progress;
    double _progress_low;
    double _progress_high;
    bool _show_text;
    sf::Text _text;
    ProgressTextStyle _text_style;
    sf::RectangleShape _main_rectangle;
    sf::RectangleShape _bar_rectangle;
};

}  // namespace VVipers
