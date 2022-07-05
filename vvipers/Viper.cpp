#include <exception>
#include <vvipers/VectorMath.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

const float Viper::s_nominalSpeed(60.f);

Viper::Viper() : m_acc(0.f), m_speed(s_nominalSpeed), m_head(nullptr) {}

Viper::~Viper() {}

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    m_vertices.draw(target, states);
}

void Viper::growth(const sf::Time& g) { m_growth += g; }

float Viper::length() const {
    return m_track.length(m_head->getTime(),
                          m_head->getTime() - m_temporalLength);
}

// Each initial segment will get the same length and be setup in a line from the
// start coordinates to the end coordinates. The track will be prepared assuming
// nominal speed. Tail at from-vector, head at to-vector.
void Viper::setup(const Vec2f& headPosition, float angle,
                  const sf::Time& viperTemporalLength) {
    logInfo("Setting up Viper.");
    const float viperLength = viperTemporalLength.asSeconds() * s_nominalSpeed;
    m_angle = angle;
    Vec2f vipVec =
        viperLength * Vec2f(cos(degToRad(angle)), sin(degToRad(angle)));
    Vec2f dL = vipVec / viperLength;
    sf::Time tailTime = sf::Time::Zero;
    sf::Time headTime = tailTime + viperTemporalLength;
    m_temporalLength = (headTime - tailTime);
    sf::Int64 numberOfPoints = 60 * m_temporalLength.asSeconds() + 1;  // 60 FPS

    // Find all the positions the Viper segments will move through
    m_track.clear();
    logInfo("Filling track with ", numberOfPoints, " track points.");
    for (int i = 0; i < numberOfPoints; ++i) {
        m_track.create_back(
            headPosition - dL * i,
            headTime - m_temporalLength * float(i) / float(numberOfPoints - 1));
    }

    m_head = m_track.front();
    m_vertices.update(m_head->getTime(), m_temporalLength, m_track);

    logInfo("Viper is ready.");
}

TrackPoint* Viper::createNextHeadTrackPoint(sf::Time elapsedTime) {
    float dx = m_speed * elapsedTime.asSeconds() * cos(degToRad(m_angle));
    float dy = m_speed * elapsedTime.asSeconds() * sin(degToRad(m_angle));
    Vec2f advance(dx, dy);
    return m_track.create_front(*m_head + advance,
                                m_head->getTime() + elapsedTime);
}

void Viper::cleanUpTrailingTrackPoints() {
    // Removes any point(s) the whole Viper has passed through
    sf::Time tailTime = m_head->getTime() - m_temporalLength;
    // Find the last two needed trackpoints
    TrackPoint* afterTail = m_track.back();
    while (afterTail && afterTail->getTime() <= tailTime)
        afterTail = afterTail->prev();
    if (!afterTail)
        throw std::runtime_error(
            "About to remove all TrackPoints, this cannot be right.");
    TrackPoint* beforeTail = afterTail->next();

    // Remove the rest
    while (beforeTail != m_track.back()) {
        m_track.pop_back();
    }
}

void Viper::grow(sf::Time elapsedTime) {
    // Limit the growth to how much time that has passed
    sf::Time actualGrowth = std::min(m_growth, elapsedTime);
    m_temporalLength += actualGrowth;
    m_growth -= actualGrowth;
}

void Viper::tick(sf::Time elapsedTime) {
    m_head = createNextHeadTrackPoint(elapsedTime);
    grow(elapsedTime);
    cleanUpTrailingTrackPoints();
    m_vertices.update(m_head->getTime(), m_temporalLength, m_track);
}

}  // namespace VVipers