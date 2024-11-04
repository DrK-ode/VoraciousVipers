#include <vvipers/GameElements/Food.hpp>
#include <vvipers/Utilities/VVColor.hpp>

namespace VVipers {

const double Food::nominalFoodRadius(15);

Food::Food(Vec2 position, double radius, Time bonusExpired, sf::Color color)
    : CircleShape(radius, 7, true),
      m_startOfDecay(0),
      m_age(0),
      m_bonusExpire(bonusExpired),
      m_originalRadius(radius) {
    auto [h, s, l] = fromRGBtoHSL(color.r, color.g, color.b);
    m_colorH = h;
    m_colorS = s;
    m_colorL = std::clamp(l, 0.3, 0.7);
    setPosition(position);
    setFillColor(color);
    setOutlineThickness(5);
}

void Food::decay(Time elapsedTime) {
    if (m_startOfDecay == timeFromSeconds(0))
        m_startOfDecay = m_age;
    auto decayTime = m_age - m_startOfDecay;
    const Time timeForDying = timeFromSeconds(0.25);

    setRadius(m_originalRadius * (timeForDying - decayTime) / timeForDying);
    if (decayTime >= timeForDying)
        state(Dead);
}

bool Food::isBonusEligible() const { return m_age < m_bonusExpire; }

double Food::getScoreValue() const {
    double score = 10. * (getRadius() * getRadius()) /
                   (nominalFoodRadius * nominalFoodRadius);
    if (isBonusEligible())
        score *= 2;
    return score;
}

sf::Color Food::getColor() const {
    return colorFromHSL(m_colorH, m_colorS, m_colorL);
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
        double L = isBonusEligible() ? m_colorL + 0.1 : m_colorL - 0.1;
        setOutlineColor(colorFromHSL(
            m_colorH, m_colorS,
            L + 0.1 * std::sin(fmod(10 * timeAsSeconds(m_age), twopi))));
    }
}

}  // namespace VVipers
