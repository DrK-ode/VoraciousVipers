#ifndef VVIPERS_FLYINGSCORE_HPP
#define VVIPERS_FLYINGSCORE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>
#include <vvipers/Time.hpp>
#include <vvipers/Vec2.hpp>
#include <vvipers/GameObject.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/GameEvent.hpp>

namespace VVipers {

class FlyingScore : public sf::Drawable, public GameObject, public Observable {
  public:
    FlyingScore(Vec2 initialPosition, Vec2 initialVelocity, Vec2 target,
               Time timeOfFlight, const score_t score);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(Time elapsedTime);

  private:
    void updateText();
    
    Vec2 m_initialPosition;  // px
    Vec2 m_initialVelocity;  // px/s
    Vec2 m_acceleration;     // px/s²
    Time m_timeOfFlight;     // s
    Time m_currentTime;      // s
    score_t m_score;
    sf::Text m_text;
    sf::Font m_font;
};

}  // namespace VVipers

#endif // VVIPERS_FLYINGSCORE_HPP
