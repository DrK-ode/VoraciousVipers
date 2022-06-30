#include "Viper.hpp"

#include <algorithm>

#include "VectorMath.hpp"
#include "debug.hpp"

const float Viper::s_segmentWidth(20);
const float Viper::s_segmentLength(30);
const float Viper::s_nominalSpeed(60.f);
const int32_t fps = 60;
const float Viper::s_pointsPerSegment(s_segmentLength / s_nominalSpeed * fps);
const uint32_t Viper::s_nVerticesHead(10);
const uint32_t Viper::s_nVerticesBody(6);
const uint32_t Viper::s_nVerticesTail(1);

Viper::Viper()
    : m_acc(0.f),
      m_color1(0x00dd00ff),
      m_color2(0x007700ff),
      m_growth(0),
      m_nSegments(0),
      m_speed(s_nominalSpeed),
      m_head(nullptr),
      m_tail(nullptr),
      m_vertices(sf::TriangleStrip) {}

Viper::~Viper() {}

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vertices, states);
}

float Viper::length() const { return m_track.size(m_head, m_tail); }

// Each initial segment will get the same length and be setup in a line from the
// start coordinates to the end coordinates. The track will be prepared assuming
// nominal speed. Tail at from-vector, head at to-vector.
void Viper::setupStart(const Vec2f& from, float angle, uint32_t nSeg) {
    logInfo("Setting up Viper.");

    m_nSegments = nSeg;

    Vec2f vipVec = s_segmentLength * m_nSegments *
                   Vec2f(cos(degToRad(angle)), sin(degToRad(angle)));

    // One point more since all segments share the end ones
    auto nTrackPoints = s_pointsPerSegment * m_nSegments + 1;

    // Find all the positions the Viper segments will move through
    m_track.clear();
    Vec2f trackVec = vipVec / (nTrackPoints - 1);

    logInfo("Filling track with ", nTrackPoints, " track points.");
    for (int i = 0; i < nTrackPoints; ++i) {
        m_track.create_back(from + vipVec - trackVec * i, angle);
    }

    m_head = m_track.front();
    m_tail = m_track.back();
    updateVertices();

    logInfo("Viper is ready.");
}

void Viper::createNextTrackPoint(sf::Time elapsedTime) {
    float dx = m_speed * elapsedTime.asSeconds() * cos(degToRad(m_angle));
    float dy = m_speed * elapsedTime.asSeconds() * sin(degToRad(m_angle));
    Vec2f advance(dx, dy);
    m_track.create_front(*m_head + advance, m_angle);
    m_angle += 0.4f;
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
    } else {  // Negative growth
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
    createNextTrackPoint(elapsedTime);
    moveHead(1);
    moveTail(1);
    cleanUpTrailingTrackPoints();
    updateVertices();
}

void Viper::updateVertices() {
    TrackPoint* segFront = m_head;
    TrackPoint* segMiddle = m_head->step(s_pointsPerSegment / 2);
    TrackPoint* segBack = m_head->step(s_pointsPerSegment);

    // Calculate segment length axis (broken in the middle)
    Vec2f dl1 = *segMiddle - *segFront;
    Vec2f dl2 = *segBack - *segMiddle;
    //
    // Calculate width (perpendicular) axis
    const float halfWidth = 0.5f * s_segmentWidth;
    Vec2f dw1 = dl1.perpVec().normalize(halfWidth);
    Vec2f dw2 = dl2.perpVec().normalize(halfWidth);

    const Vec2f dw_avg = (dw1 + dw2) / 2.f;  // Helper
    int nVertices = s_nVerticesHead;
    if (m_nSegments > 1)
        nVertices += s_nVerticesTail;
    if (m_nSegments > 2)
        nVertices += s_nVerticesBody * (m_nSegments - 2);
    m_vertices.resize(nVertices);
    int i = 0;
    // Draw head
    m_vertices[i].position = *segFront - dw1 / 3.f;
    m_vertices[i++].color = m_color1;
    m_vertices[i].position = *segFront + dw1 / 3.f;
    m_vertices[i++].color = m_color1;

    m_vertices[i].position = *segFront + 0.8f * dl1 - dw_avg;
    m_vertices[i++].color = m_color2;
    m_vertices[i].position = *segFront + 0.8f * dl1 + dw_avg;
    m_vertices[i++].color = m_color2;
    m_vertices[i].position = *segBack - 0.8f * dl2 - dw_avg;
    m_vertices[i++].color = m_color2;
    m_vertices[i].position = *segBack - 0.8f * dl2 + dw_avg;
    m_vertices[i++].color = m_color2;

    m_vertices[i].position = *segBack - 0.2f * dl2 - 0.5f * dw2;
    m_vertices[i++].color = m_color1;
    m_vertices[i].position = *segBack - 0.2f * dl2 + 0.5f * dw2;
    m_vertices[i++].color = m_color1;
    m_vertices[i].position = *segBack - 2.f / 3.f * dw2;
    m_vertices[i++].color = m_color1;
    m_vertices[i].position = *segBack + 2.f / 3.f * dw2;
    m_vertices[i++].color = m_color1;
    // Check for stupid mistakes
    if (i != s_nVerticesHead)
        logError("Wrong number of head vertices: ", i, ".");

    if (m_nSegments > 2) {
        for (int n = 0; n < m_nSegments - 2; ++n) {
            segFront = segBack;
            segMiddle = segFront->step(s_pointsPerSegment / 2);
            segBack = segBack->step(s_pointsPerSegment);
            // Calculate segment length axis (broken in the middle)
            Vec2f dl1 = *segMiddle - *segFront;
            Vec2f dl2 = *segBack - *segMiddle;
            //
            // Calculate width (perpendicular) axis
            const float halfWidth = 0.5f * s_segmentWidth;
            Vec2f dw1 = dl1.perpVec().normalize(halfWidth);
            Vec2f dw2 = dl2.perpVec().normalize(halfWidth);

            const Vec2f dw_avg = (dw1 + dw2) / 2.f;  // Helper
            // Draw body
            m_vertices[i].position = *segFront + 0.5f * dl1 - dw_avg;
            m_vertices[i++].color = m_color2;
            m_vertices[i].position = *segFront + 0.5f * dl1 + dw_avg;
            m_vertices[i++].color = m_color2;
            m_vertices[i].position = *segBack - 0.5f * dl2 - dw_avg;
            m_vertices[i++].color = m_color2;
            m_vertices[i].position = *segBack - 0.5f * dl2 + dw_avg;
            m_vertices[i++].color = m_color2;
            m_vertices[i].position = *segBack - dw2 * (2.f / 3.f);
            m_vertices[i++].color = m_color1;
            m_vertices[i].position = *segBack + dw2 * (2.f / 3.f);
            m_vertices[i++].color = m_color1;
        }
        // Check for stupid mistakes
        if (i != s_nVerticesHead + s_nVerticesBody * (m_nSegments - 2))
            logError("Wrong number of body vertices: ", i - s_nVerticesHead,
                     ".");
    }

    if (m_nSegments > 1) {
        segFront = segBack;
        segMiddle = segFront->step(s_pointsPerSegment / 2);
        segBack = segBack->step(s_pointsPerSegment);
        // Draw tail
        m_vertices[i].position = *segBack;
        m_vertices[i++].color = m_color2;

        // Check for stupid mistakes
        if (i != s_nVerticesHead + s_nVerticesBody * (m_nSegments - 2) +
                     s_nVerticesTail)
            logError("Wrong number of tail vertices: ",
                     i - s_nVerticesHead - s_nVerticesBody * (m_nSegments - 2),
                     ".");
    }
}