#include <vvipers/Viper.hpp>

#include <vvipers/VectorMath.hpp>
#include <vvipers/debug.hpp>

const float Viper::s_nominalSpeed(60.f);
const uint32_t fps = 60;
const uint32_t Viper::s_nPtsPerSegment(ViperVertices::getSegmentLength() /
                                       s_nominalSpeed * fps);

Viper::Viper()
    : m_acc(0.f),
      m_growth(0),
      m_speed(s_nominalSpeed),
      m_head(nullptr),
      m_tail(nullptr) {}

Viper::~Viper() {}

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    m_vertices.draw(target, states);
}

void Viper::growSegment(float growth) {
    static float fraction = 0;
    float wholePts;
    fraction = std::modf(growth * s_nPtsPerSegment + fraction, &wholePts);
    m_growth += wholePts;
}

float Viper::length() const { return m_track.length(m_head, m_tail); }

// Each initial segment will get the same length and be setup in a line from the
// start coordinates to the end coordinates. The track will be prepared assuming
// nominal speed. Tail at from-vector, head at to-vector.
void Viper::setup(const Vec2f& headPosition, float angle, uint32_t nSeg) {
    logInfo("Setting up Viper.");
    m_angle = angle;
    Vec2f vipVec = ViperVertices::getSegmentLength() * nSeg *
                   Vec2f(cos(degToRad(angle)), sin(degToRad(angle)));

    // One point more since all segments share the end ones
    uint32_t nTrackPoints = s_nPtsPerSegment * nSeg + 1;

    // Find all the positions the Viper segments will move through
    m_track.clear();
    Vec2f trackVec = vipVec / (nTrackPoints - 1);

    logInfo("Filling track with ", nTrackPoints, " track points.");
    for (int i = 0; i < nTrackPoints; ++i) {
        m_track.create_back(headPosition - trackVec * i);
    }

    m_head = m_track.front();
    m_tail = m_track.back();
    m_vertices.update(m_head, m_tail, s_nPtsPerSegment);

    logInfo("Viper is ready.");
}

void Viper::createNextTrackPoint(sf::Time elapsedTime) {
    float dx = m_speed * elapsedTime.asSeconds() * cos(degToRad(m_angle));
    float dy = m_speed * elapsedTime.asSeconds() * sin(degToRad(m_angle));
    Vec2f advance(dx, dy);
    m_track.create_front(*m_head + advance);
}

void Viper::moveHead(int frames) {
    m_head = m_head->step(
        -frames);  // Always moves head one track point further per frame
}

void Viper::moveTail(int frames) {
    // Default: moves tail one track point further per frame, i.e., towards the
    // front on the track
    int tail_traverse = -frames;
    if (m_growth > 0) {
        // Tail is moving one point less so the Viper has grown one track point
        ++tail_traverse;
        --m_growth;
    } else if (m_growth < 0) {  // Negative growth
        ++tail_traverse;
        ++m_growth;
        tagWarning("Negative growth should not happen yet.");
    }
    m_tail = m_tail->step(tail_traverse);
}

void Viper::cleanUpTrailingTrackPoints() {
    while (m_track.back() != m_tail) {
        m_track.pop_back();
    }
}

void Viper::tick(sf::Time elapsedTime) {
    static sf::Time t = sf::Time::Zero;
    t += elapsedTime;
    createNextTrackPoint(elapsedTime);
    moveHead(1);
    moveTail(1);
    cleanUpTrailingTrackPoints();
    m_vertices.update(m_track.front(), m_track.back(), s_nPtsPerSegment);
}
