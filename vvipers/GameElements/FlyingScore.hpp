#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/GameElements/GameEvent.hpp>
#include <vvipers/GameElements/GameObject.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

class FontProvider;

class FlyingScore : public sf::Drawable,
                    public GameObject,
                    public Observable,
                    public Observer {
  public:
    FlyingScore(Vec2 initialPosition, Vec2 initialVelocity, Vec2 target,
                Time timeOfFlight, const uint64_t score,
                const FontProvider& fontProvider);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void on_notify(const GameEvent&) override;
    void update(Time elapsedTime);
    void set_color(sf::Color fillColor,
                   sf::Color outlineColor = sf::Color::Transparent);
    void set_font_size(unsigned int characterSize,
                       double outlineThickness = 0.);

  private:
    void update_text();

    Vec2 _initial_position;  // px
    Vec2 _initial_velocity;  // px/s
    Vec2 _acceleration;      // px/s²
    Time _time_of_flight;    // s
    Time _current_time;      // s
    uint64_t _score;
    sf::Text _text;
    const sf::Font* _font;
};

}  // namespace VVipers
