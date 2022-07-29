#include <SFML/Graphics/RenderTarget.hpp>
#include <sstream>
#include <vvipers/FlyingScore.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/GameOptions.hpp>

namespace VVipers {

FlyingScore::FlyingScore(Vec2 initialPosition, Vec2 initialVelocity,
                         Vec2 target, Time timeOfFlight, score_t score)
    : m_initialPosition(initialPosition),
      m_initialVelocity(initialVelocity),
      m_timeOfFlight(timeOfFlight),
      m_currentTime(0),
      m_score(score) {
    auto tof = toSeconds(m_timeOfFlight);
    // Constant acceleration
    m_acceleration = 2 *
                     (target - m_initialPosition - m_initialVelocity * tof) /
                     (tof * tof);
    // Load and set font
    std::stringstream ss;
        ss << GameOptions::getOptionString("General/resourceDirectoryPath")
           << GameOptions::getOptionString("General/fontFileName");
    m_font.loadFromFile( ss.str() );
    m_text.setFont(m_font);
    updateText();
    auto lb = m_text.getLocalBounds();
    m_text.setOrigin(0.5 * (lb.left + lb.width), 0.5 * (lb.top + lb.height));
}

void FlyingScore::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_text, states);
}

void FlyingScore::updateText() {
    std::stringstream ss;
    ss << '+' << m_score;
    m_text.setString(ss.str());
}

void FlyingScore::update(Time elapsedTime) {
    if (state() == Dead) {
        DestroyEvent event(this);
        notify(&event);
        return;
    }
    if (state() == Dying)
        state(Dead);
    else {
        m_currentTime += elapsedTime;
        auto t = toSeconds(m_currentTime);
        auto currentPosition = m_initialPosition + m_initialVelocity * t +
                               0.5 * m_acceleration * t * t;
        m_text.setPosition(currentPosition);
        if (m_currentTime >= m_timeOfFlight) {
            state(Dying);
            ScoringEvent event(nullptr, m_score);
            notify(&event);
        }
    }
}

void FlyingScore::setColor(sf::Color fillColor, sf::Color outlineColor) {
    m_text.setFillColor(fillColor);
    m_text.setOutlineColor(outlineColor);
}

void FlyingScore::setFontSize(unsigned int characterSize,
                              double outlineThickness) {
    m_text.setCharacterSize(characterSize);
    m_text.setOutlineThickness(outlineThickness);
}

}  // namespace VVipers