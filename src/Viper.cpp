#include "Viper.hpp"

#include <algorithm>

#include "VectorMath.hpp"
#include "debug.hpp"

const float Viper::s_segmentWidth(20);
const float Viper::s_segmentLength(30);
const float Viper::s_nominalSpeed(60.f);
const uint32_t fps = 60;
const uint32_t Viper::s_nPtsPerSegment(s_segmentLength / s_nominalSpeed * fps);
const uint32_t Viper::s_nVerticesHead(10);
const uint32_t Viper::s_nVerticesBody(6);
const uint32_t Viper::s_nVerticesTail(1);

Viper::Viper()
    : m_acc(0.f),
      m_colors {sf::Color(0x00dd00ff), sf::Color(0x007700ff)},
      m_growth(0),
      m_speed(s_nominalSpeed),
      m_head(nullptr),
      m_tail(nullptr) {}

Viper::~Viper() {}

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(&m_vertices[0], m_vertices.size(), sf::TriangleStrip, states);
}

float Viper::length() const { return m_track.length(m_head, m_tail); }

// Each initial segment will get the same length and be setup in a line from the
// start coordinates to the end coordinates. The track will be prepared assuming
// nominal speed. Tail at from-vector, head at to-vector.
void Viper::setupStart(const Vec2f& headPosition, float angle, uint32_t nSeg) {
    logInfo("Setting up Viper.");

    Vec2f vipVec = s_segmentLength * nSeg *
                   Vec2f(cos(degToRad(angle)), sin(degToRad(angle)));

    // One point more since all segments share the end ones
    auto nTrackPoints = s_nPtsPerSegment * nSeg + 1;

    // Find all the positions the Viper segments will move through
    m_track.clear();
    Vec2f trackVec = vipVec / (nTrackPoints - 1);

    logInfo("Filling track with ", nTrackPoints, " track points.");
    for (int i = 0; i < nTrackPoints; ++i) {
        m_track.create_back(headPosition - trackVec * i);
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
    m_track.create_front(*m_head + advance);
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
    } else if( m_growth < 0 ) {  // Negative growth
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
    if (t.asSeconds() > 1) {
        m_growth += s_nPtsPerSegment;
        t = sf::Time::Zero;
    }
    tagInfo("dist to end: ", m_track.size(m_tail,nullptr), ", m_tail->next: ", m_tail->next(),", size: ", m_track.size());
    createNextTrackPoint(elapsedTime);
    moveHead(1);
    moveTail(1);
    cleanUpTrailingTrackPoints();
    updateVertices();
}

void Viper::setHeadVertices(TrackPoint* tp_front, TrackPoint* tp_back,
                            sf::Vertex array[]) {
    TrackPoint* spine[s_nVerticesHead / 2];
    spine[0] = tp_front;
    spine[1] = spine[0]->step(s_nPtsPerSegment * 3 / 8);
    spine[2] = spine[1]->step(s_nPtsPerSegment / 4);
    spine[3] = spine[2]->step(s_nPtsPerSegment / 4);
    spine[4] = tp_back;
    float hwidth[s_nVerticesHead / 2];
    hwidth[0] = s_segmentWidth * 0.15f;
    hwidth[1] = s_segmentWidth * 0.45f;
    hwidth[2] = s_segmentWidth * 0.45f;
    hwidth[3] = s_segmentWidth * 0.2f;
    hwidth[4] = s_segmentWidth * 0.2f;
    Vec2f arm[s_nVerticesHead / 2];
    for (int i = 0; i < s_nVerticesHead/2; ++i) 
        arm[i] = (*(spine[i]->next()) - *(spine[i])).perpVec().normalize(hwidth[i]);
    // Since there is a tail there will always be a next point after the head.

    for (int i = 0; i < s_nVerticesHead; ++i) {
        array[i].position = *spine[i / 2] - (1 - 2*(i%2)) * arm[i / 2];
        array[i].color = m_colors[(i % 4) / 2];
    }
}

void Viper::setBodyVertices(TrackPoint* tp_front, TrackPoint* tp_back,
                            sf::Vertex array[]) {
    TrackPoint* spine[s_nVerticesBody / 2];
    spine[0] = tp_front->step(s_nPtsPerSegment / 3);
    spine[1] = spine[0]->step(s_nPtsPerSegment / 3);
    spine[2] = tp_back;
    float hwidth[s_nVerticesBody / 2];
    hwidth[0] = s_segmentWidth * 0.5f;
    hwidth[1] = s_segmentWidth * 0.5f;
    hwidth[2] = s_segmentWidth * 0.2f;
    Vec2f arm[s_nVerticesBody / 2];
    for (int i = 0; i < s_nVerticesBody/2; ++i) 
        arm[i] = (*(spine[i]->next()) - *(spine[i])).perpVec().normalize(hwidth[i]);

    for (int i = 0; i < s_nVerticesBody; ++i) {
        array[i].position = *spine[i / 2] - (1 - 2*(i%2))*arm[i / 2];
        array[i].color = m_colors[(i % 4) / 2];
    }
}

void Viper::setTailVertices(TrackPoint* tp_front, TrackPoint* tp_back,
                            sf::Vertex array[]) {
    // Draw tail
    array[0].position = *tp_back;
    array[0].color = m_colors[1];
}

void Viper::updateVertices() {
    size_t viperSize = m_track.size() - 1; // One less since we are counting intervals, not points
    if (viperSize < 2*s_nPtsPerSegment) {
        tagError("Viper is too small to afford a head and a tail.");
        throw std::runtime_error("Inacceptable viper size.");
    }
    int32_t nHeadPts = s_nPtsPerSegment;
    int32_t nBodySegments =
        std::max(0uL, (viperSize - nHeadPts) / s_nPtsPerSegment - 1);
    int32_t nBodyPts = nBodySegments * s_nPtsPerSegment;
    int32_t nTailPts = viperSize - nHeadPts - nBodyPts;
    // Sanity check
    if (nBodyPts > 0 && nTailPts < s_nPtsPerSegment) {
        tagError("Tail is too small even though is could be bigger.");
        throw std::runtime_error("Inacceptable tail size.");
    }
    int nVertices =
        s_nVerticesHead + s_nVerticesBody * nBodySegments + s_nVerticesTail;
    m_vertices.resize(nVertices);

    TrackPoint* segFront = m_track.front();
    TrackPoint* segBack = m_track.front()->step(s_nPtsPerSegment);
    sf::Vertex* storagePtr = &m_vertices[0];
    setHeadVertices(segFront,segBack, storagePtr);
    storagePtr += s_nVerticesHead;

    for (int i = 0; i < nBodySegments; ++i) {
        segFront = segBack;
        segBack = segFront->step(s_nPtsPerSegment);
        setBodyVertices(segFront, segBack, storagePtr);
        storagePtr += s_nVerticesBody;
    }

    segFront = segBack;
    segBack = segFront->step(nTailPts);
    setTailVertices(segFront, segBack, storagePtr);
}