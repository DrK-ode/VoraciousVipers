#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/FlyingScore.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/config.hpp>

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
    m_font.loadFromFile(FONT_FILE_PATH);
    m_text.setFont(m_font);
    updateText();
}

void FlyingScore::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_text, states);
    tagDebug("Drawing ", m_text.getString().operator std::string(), " at ", Vec2(m_text.getPosition()));
}

void FlyingScore::updateText() {
    std::stringstream ss;
    ss << '+' << m_score;
    m_text.setString(ss.str());
    m_text.setFillColor(sf::Color::Magenta);
    m_text.setCharacterSize(20);
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
        tagDebug(m_currentTime);
        auto t = toSeconds(m_currentTime);
        auto currentPosition = m_initialPosition + m_initialVelocity * t +
                               0.5 * m_acceleration * t * t;
        tagDebug(currentPosition);
        m_text.setPosition(currentPosition);
        if (m_currentTime >= m_timeOfFlight) {
            state(Dying);
            ScoringEvent event(nullptr, m_score);
            notify(&event);
        }
    }
}

}  // namespace VVipers