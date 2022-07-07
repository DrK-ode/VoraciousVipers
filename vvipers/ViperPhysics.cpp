#include <exception>
#include <vvipers/Vec2.hpp>
#include <vvipers/ViperPhysics.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

const double ViperPhysics::s_nominalSpeed(60.);

ViperPhysics::ViperPhysics()
    : m_acc(0.), m_speed(s_nominalSpeed), m_growth(0), m_head(nullptr) {}

void ViperPhysics::growth(const Time& g) { m_growth += g; }

double ViperPhysics::length() const {
    return m_track.length(m_head->getTime(),
                          m_head->getTime() - m_temporalLength);
}

// Each initial segment will get the same length and be setup in a line from the
// start coordinates to the end coordinates. The track will be prepared assuming
// nominal speed. Tail at from-vector, head at to-vector.
void ViperPhysics::setup(const Vec2& headPosition, double angle,
                         const Time& viperTemporalLength) {
    logInfo("Setting up physical viper.");
    const double viperLength = toSeconds(viperTemporalLength) * s_nominalSpeed;
    m_angle = angle;
    Vec2 vipVec =
        viperLength * Vec2(cos(degToRad(angle)), sin(degToRad(angle)));
    Vec2 dL = vipVec / viperLength;
    Time tailTime = seconds(0);;
    Time headTime = tailTime + viperTemporalLength;
    m_temporalLength = (headTime - tailTime);
    size_t numberOfPoints = 60 * toSeconds(m_temporalLength) + 1;  // 60 FPS

    // Find all the positions the Viper segments will move through
    m_track.clear();
    logInfo("Filling track with ", numberOfPoints, " track points.");
    for (int i = 0; i < numberOfPoints; ++i) {
        m_track.create_back(
            headPosition - dL * i,
            headTime - (m_temporalLength * i) / (numberOfPoints - 1));
    }

    m_head = m_track.front();

    logInfo("Physical viper is ready.");
}

TrackPoint* ViperPhysics::createNextHeadTrackPoint(Time elapsedTime) {
    double dx = m_speed * toSeconds(elapsedTime) * cos(degToRad(m_angle));
    double dy = m_speed * toSeconds(elapsedTime) * sin(degToRad(m_angle));
    Vec2 advance(dx, dy);
    return m_track.create_front(*m_head + advance,
                                m_head->getTime() + elapsedTime);
}

void ViperPhysics::cleanUpTrailingTrackPoints() {
    // Removes any point(s) the whole Viper has passed through
    Time tailTime = m_head->getTime() - m_temporalLength;
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

void ViperPhysics::grow(const Time& elapsedTime) {
    // Limit the growth to how much time that has passed
    Time actualGrowth = std::min(m_growth, elapsedTime);
    m_temporalLength += actualGrowth;
    m_growth -= actualGrowth;

}

void ViperPhysics::update(const Time& elapsedTime) {
    m_head = createNextHeadTrackPoint(elapsedTime);
    grow(elapsedTime);
    cleanUpTrailingTrackPoints();
}

}  // namespace VVipers