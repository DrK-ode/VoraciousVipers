#include "Viper.hpp"

#include <algorithm>

#include "VectorMath.hpp"
#include "debug.hpp"

Viper::Viper() {
    m_nominalSpeed = 60.f;
    m_speed = m_nominalSpeed;
    m_angle = 45.f;
    m_acc = 0.f;
}

Viper::~Viper() {
    for_each(m_segments.begin(), m_segments.end(),
             [](ViperSegment* vs) { delete vs; });
}

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for_each(m_segments.begin(), m_segments.end(),
             [&](const ViperSegment* seg) { target.draw(*seg, states); });
}

float Viper::length() const {
    return m_track.length(head()->getPositionBegin(), tail()->getPositionEnd());
}

// Each initial segment will get the same length and be setup in a line from the
// start coordinates to the end coordinates. The track will be prepared assuming
// nominal speed. Tail at from-vector, head at to-vector.
void Viper::setupStart(const Vec2f& from, const Vec2f& to, uint32_t nSeg) {
    logInfo("Setting up Viper.");

    Vec2f vipVec = (from - to);
    Vec2f segVec = vipVec / nSeg;
    float angle = radToDeg( atan2( vipVec.y, vipVec.x ) );

    // First, find all the positions the Viper segments will move through
    m_track.clear();
    const uint32_t fps = 60;
    uint32_t nSegIntervals = segVec.abs() / m_nominalSpeed * fps;
    Vec2f trackVec = segVec / nSegIntervals;
    // One point more than the number of track intervals
    auto nTrackPoints = nSegIntervals * nSeg + 1;
    logInfo("Filling track with ", nTrackPoints, " track points.");
    for (int i = 0; i < nTrackPoints; ++i) {
        m_track.create_back( to + trackVec * i, angle );
    }

    // Second, construct the segments and line them up
    m_segments.clear();
    m_segments.push_back(new ViperSegment);
    for (int i = 1; i < nSeg; ++i) {
        ViperSegment* seg = new ViperSegment;
        // Attach the new segment to the previous one
        m_segments.back()->attach(seg);
        m_segments.push_back(seg);
    }
    ViperSegment* seg = head();
    TrackPoint* tp = m_track.front();
    logInfo("First track point at ", *tp);
    while (seg) {
        TrackPoint* middle = tp->traverse(int(0.5 * nSegIntervals));
        TrackPoint* end = tp->traverse(nSegIntervals);
        seg->setTrackPoints(tp, middle, end);
        tp = end;
        seg = seg->next();
    }
    logInfo("Last track point at ", *tp );
    logInfo("Viper is ready.");
}

void Viper::tick(sf::Time elapsedTime) {
    logInfo("Viper tick.");
    const ViperSegment* head = m_segments[0];
    float dx = m_speed * elapsedTime.asSeconds() * cos(degToRad(m_angle));
    float dy = m_speed * elapsedTime.asSeconds() * sin(degToRad(m_angle));
    Vec2f advance(dx, dy);
    m_track.create_front(*(head->getPositionBegin()) + advance, m_angle);
    m_angle += 0.4f;

    for_each(m_segments.begin(), m_segments.end(),
             [&](ViperSegment* vs) { vs->step(1); });

    m_track.pop_back();
    logInfo("Viper tock.");
}