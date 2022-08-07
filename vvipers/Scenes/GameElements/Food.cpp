#include <vvipers/Scenes/GameElements/Food.hpp>
#include <vvipers/Utilities/VVColor.hpp>

namespace VVipers {

const double Food::nominalFoodRadius(20);

Food::Food(Vec2 position, double radius, Time bonusExpired)
    : CircleShape(radius, 7, true),
      m_startOfDecay(0),
      m_age(0),
      m_bonusExpire(bonusExpired),
      m_originalRadius(radius),
      m_hue(Random::getDouble()) {
    setPosition(position);
    setFillColor(sf::Color::Cyan);
    setOutlineColor(sf::Color::Magenta);
    setOutlineThickness(1);
}

void Food::decay(Time elapsedTime) {
    if (m_startOfDecay == seconds(0))
        m_startOfDecay = m_age;

    auto decayTime = m_age - m_startOfDecay;

    // const double twirl = 5 * 360;  // deg / s
    const Time timeForDying = seconds(0.25);

    // rotate(twirl * toSeconds(elapsedTime));
    setRadius(m_originalRadius * (timeForDying - decayTime) / timeForDying);
    if (decayTime >= timeForDying)
        state(Dead);
}

bool Food::isBonusEligible() const { return m_age < m_bonusExpire; }

double Food::getScoreValue() const {
    double score = 10. * (getRadius() * getRadius()) / nominalFoodRadius *
                   nominalFoodRadius;
    if (isBonusEligible())
        score *= 2;
    return score;
}

void Food::update(Time elapsedTime) {
    m_age += elapsedTime;
    rotate(2 * nominalFoodRadius / getRadius());
    if (state() == Dead) {
        DestroyEvent event(this);
        notify(&event);
        return;
    }
    if (state() == Dying)
        decay(elapsedTime);
    else {
        if (isBonusEligible()) {
            setFillColor(colorFromHSL(
                m_hue, 0.5,
                0.5 + 0.25 * std::sin(fmod(10 * toSeconds(m_age), twopi))));
        } else {
            setFillColor(colorFromHSL(m_hue, 0.5, 0.5));
        }
    }
}

}  // namespace VVipers
