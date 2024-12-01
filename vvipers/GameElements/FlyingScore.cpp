#include <SFML/Graphics/RenderTarget.hpp>
#include <sstream>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/GameElements/FlyingScore.hpp>
#include <vvipers/GameElements/GameEvent.hpp>

namespace VVipers {

FlyingScore::FlyingScore(Vec2 initial_position, Vec2 initial_velocity,
                         Vec2 target, Time time_of_flight, uint64_t score,
                         const FontProvider& font_provider)
    : _initial_position(initial_position),
      _initial_velocity(initial_velocity),
      _time_of_flight(time_of_flight),
      _current_time(0),
      _score(score) {
    auto tof = time_as_seconds(_time_of_flight);
    // Constant acceleration
    _acceleration = 2 * (target - _initial_position - _initial_velocity * tof) /
                    (tof * tof);
    // Load and set font
    _font = font_provider.default_font();
    _text.setFont(*_font);
    update_text();
    auto bounds = _text.getLocalBounds();
    _text.setOrigin(0.5 * (bounds.left + bounds.width),
                    0.5 * (bounds.top + bounds.height));
}

void FlyingScore::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(_text, states);
}

void FlyingScore::update_text() {
    std::stringstream ss;
    ss << '+' << _score;
    _text.setString(ss.str());
}

void FlyingScore::update(Time elapsedTime) {
    if (state() == Dead) {
        notify(DestroyEvent(this));
        return;
    }
    if (state() == Dying)
        state(Dead);
    else {
        _current_time += elapsedTime;
        auto t = time_as_seconds(_current_time);
        auto currentPosition = _initial_position + _initial_velocity * t +
                               0.5 * _acceleration * t * t;
        _text.setPosition(currentPosition);
        if (_current_time >= _time_of_flight) {
            state(Dying);
            notify(ScoringEvent(_score));
        }
    }
}

void FlyingScore::set_color(sf::Color fill_color, sf::Color outline_color) {
    _text.setFillColor(fill_color);
    _text.setOutlineColor(outline_color);
}

void FlyingScore::set_font_size(unsigned int character_size,
                                double outline_thickness) {
    _text.setCharacterSize(character_size);
    _text.setOutlineThickness(outline_thickness);
}

}  // namespace VVipers