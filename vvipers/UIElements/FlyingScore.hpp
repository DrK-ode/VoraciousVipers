#ifndef VVIPERS_UIELEMENTS_FLYINGSCORE_HPP
#define VVIPERS_UIELEMENTS_FLYINGSCORE_HPP

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

class FlyingScore : public sf::Drawable, public GameObject, public Observable {
  public:
    FlyingScore(Vec2 initialPosition, Vec2 initialVelocity, Vec2 target,
                Time timeOfFlight, const uint64_t score, const FontProvider& fontProvider);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(Time elapsedTime);
    void setColor(sf::Color fillColor,
                  sf::Color outlineColor = sf::Color::Transparent);
    void setFontSize(unsigned int characterSize, double outlineThickness = 0.);

  private:
    void updateText();

    Vec2 m_initialPosition;  // px
    Vec2 m_initialVelocity;  // px/s
    Vec2 m_acceleration;     // px/s²
    Time m_timeOfFlight;     // s
    Time m_currentTime;      // s
    uint64_t m_score;
    sf::Text m_text;
    const sf::Font* m_font;
};

}  // namespace VVipers

#endif // VVIPERS_UIELEMENTS_FLYINGSCORE_HPP
