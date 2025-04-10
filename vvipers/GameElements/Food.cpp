#include <SFML/Graphics/CircleShape.hpp>
#include <memory>
#include <stdexcept>
#include <vvipers/GameElements/Food.hpp>
#include <vvipers/Utilities/VVColor.hpp>

#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/GameElements/GameEvent.hpp"

namespace VVipers {

const double Food::nominal_food_radius(15);

Food::Food(Vec2 position, double radius, Time bonusExpired, sf::Color color)
    : sf::CircleShape(radius, 7),
      CollidingBody("Food"),
      _age(0),
      _bonus_expire(bonusExpired),
      _original_radius(radius),
      _start_of_decay(0) {
    auto [h, s, l] = hsl_from_rgb(color.r, color.g, color.b);
    _color_hue = h;
    _color_saturation = s;
    _color_lightness = std::clamp(l, 0.3, 0.7);
    sf::CircleShape::setPosition(position);
    sf::CircleShape::setOrigin(radius, radius);
    sf::CircleShape::setFillColor(color);
    sf::CircleShape::setOutlineThickness(5);
    _shape = std::make_shared<Circle>(CircleShape::getPosition(),
                                      CircleShape::getRadius());
}

std::shared_ptr<const VVipers::Shape> Food::segment_shape(size_t index) const {
    if (index >= number_of_segments())
        throw std::runtime_error("Requested body part index is too large.");
    return _shape;
}

sf::Color Food::color() const {
    return color_from_hsl(_color_hue, _color_saturation, _color_lightness);
}

void Food::decay(Time elapsedTime) {
    if (_start_of_decay == time_from_seconds(0))
        _start_of_decay = _age;
    auto decayTime = _age - _start_of_decay;
    const Time timeForDying = time_from_seconds(0.25);

    setRadius(_original_radius * (timeForDying - decayTime) / timeForDying);
    if (decayTime >= timeForDying)
        state(Dead);
}

bool Food::is_bonus_eligible() const { return _age < _bonus_expire; }

double Food::score_value() const {
    double score =
        10. * (sf::CircleShape::getRadius() * sf::CircleShape::getRadius()) /
        (nominal_food_radius * nominal_food_radius);
    if (is_bonus_eligible())
        score *= 2;
    return score;
}

void Food::update(Time elapsed_time) {
    _age += elapsed_time;
    rotate(2 * nominal_food_radius / sf::CircleShape::getRadius());
    if (state() == Dead) {
        notify(DestroyEvent(this));
        return;
    }
    if (state() == Dying)
        decay(elapsed_time);
    else {
        double L = is_bonus_eligible() ? _color_lightness + 0.1
                                       : _color_lightness - 0.1;
        sf::CircleShape::setOutlineColor(color_from_hsl(
            _color_hue, _color_saturation,
            L + 0.1 * std::sin(fmod(10 * time_as_seconds(_age), twopi))));
    }
}

}  // namespace VVipers
