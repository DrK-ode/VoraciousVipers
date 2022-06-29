#include "ViperSegment.hpp"

#include "debug.hpp"

using sf::Color;

const float ViperSegment::s_lengthWidthRatio = 0.75;
const Color ViperSegment::s_color1(0x00ff00ff);
const Color ViperSegment::s_color2(0x009900ff);

ViperSegment::ViperSegment()
    : m_posBegin(),
      m_posMiddle(),
      m_posEnd(),
      m_prev(nullptr),
      m_next(nullptr),
      m_vertices(sf::TriangleStrip) {}

ViperSegment::~ViperSegment() {}

void ViperSegment::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
    target.draw(m_vertices, states);
}

void ViperSegment::attach(ViperSegment* seg) {
    this->m_next = seg;
    seg->m_prev = this;
}

ViperSegment* ViperSegment::next() const { return m_next; }
ViperSegment* ViperSegment::prev() const { return m_prev; }

size_t ViperSegment::distToHead() const {
    if (m_prev == nullptr)
        return 0;
    else
        return m_prev->distToHead() + 1;
}

size_t ViperSegment::distToTail() const {
    if (m_next == nullptr)
        return 0;
    else
        return m_next->distToTail() + 1;
}

ViperSegment::SegmentType ViperSegment::segmentType() const {
    if (m_next != nullptr && m_prev != nullptr)
        return ViperSegment::SegmentType::Body;
    else if (m_prev != nullptr)
        return ViperSegment::SegmentType::Tail;
    else
        return ViperSegment::SegmentType::Head;
}

void ViperSegment::setTrackPoints(TrackPoint* begin, TrackPoint* middle,
                                  TrackPoint* end) {
    // Save values in object
    m_posBegin = begin;
    m_posMiddle = middle;
    m_posEnd = end;
    if (!m_posBegin || !m_posMiddle || !m_posEnd)
        throw std::domain_error(
            "Error: ViperSegment TrackPoint pointer cannot be nullptr.");
    updateVertices();
}

void ViperSegment::updateVertices() {
    // Calculate segment length axis (broken in the middle)
    Vec2f dl1 = *m_posMiddle - *m_posBegin;
    Vec2f dl2 = *m_posEnd - *m_posMiddle;
    //
    float length = dl1.abs() + dl2.abs();
    float width = length * s_lengthWidthRatio;
    // Calculate width (perpendicular) axis
    const float halfWidth = 0.5f * width;
    Vec2f dw1 = dl1.perpVec().normalize(halfWidth);
    Vec2f dw2 = dl2.perpVec().normalize(halfWidth);

    const Vec2f dw_avg = (dw1 + dw2) / 2.f;  // Helper

    switch (segmentType()) {
        case ViperSegment::SegmentType::Head: {
            m_vertices.resize(10);
            m_vertices[0].position = *m_posBegin - dw1 / 3.f;
            m_vertices[1].position = *m_posBegin + dw1 / 3.f;
            m_vertices[2].position = *m_posBegin + 0.8f * dl1 - dw_avg;
            m_vertices[3].position = *m_posBegin + 0.8f * dl1 + dw_avg;
            m_vertices[4].position = *m_posEnd - 0.8f * dl2 - dw_avg;
            m_vertices[5].position = *m_posEnd - 0.8f * dl2 + dw_avg;
            m_vertices[6].position = *m_posEnd - 0.2f * dl2 - 0.5f * dw2;
            m_vertices[7].position = *m_posEnd - 0.2f * dl2 + 0.5f * dw2;
            m_vertices[8].position = *m_posEnd - 2.f / 3.f * dw2;
            m_vertices[9].position = *m_posEnd + 2.f / 3.f * dw2;

            m_vertices[0].color = s_color1;
            m_vertices[1].color = s_color1;
            m_vertices[2].color = s_color2;
            m_vertices[3].color = s_color2;
            m_vertices[4].color = s_color2;
            m_vertices[5].color = s_color2;
            m_vertices[6].color = s_color1;
            m_vertices[7].color = s_color1;
            m_vertices[8].color = s_color1;
            m_vertices[9].color = s_color1;
            break;
        }
        case ViperSegment::SegmentType::Tail: {
            m_vertices.resize(3);
            m_vertices[0].position = *m_posBegin - dw1 * (2.f / 3.f);
            m_vertices[1].position = *m_posBegin + dw1 * (2.f / 3.f);
            m_vertices[2].position = *m_posEnd;

            m_vertices[0].color = s_color1;
            m_vertices[1].color = s_color1;
            m_vertices[2].color = s_color2;
            break;
        }
        case ViperSegment::SegmentType::Body: {
            m_vertices.resize(8);
            m_vertices[0].position = *m_posBegin - dw1 * (2.f / 3.f);
            m_vertices[1].position = *m_posBegin + dw1 * (2.f / 3.f);
            m_vertices[2].position = *m_posBegin + 0.5f * dl1 - dw_avg;
            m_vertices[3].position = *m_posBegin + 0.5f * dl1 + dw_avg;
            m_vertices[4].position = *m_posEnd - 0.5f * dl2 - dw_avg;
            m_vertices[5].position = *m_posEnd - 0.5f * dl2 + dw_avg;
            m_vertices[6].position = *m_posEnd - dw2 * (2.f / 3.f);
            m_vertices[7].position = *m_posEnd + dw2 * (2.f / 3.f);

            m_vertices[0].color = s_color1;
            m_vertices[1].color = s_color1;
            m_vertices[2].color = s_color2;
            m_vertices[3].color = s_color2;
            m_vertices[4].color = s_color2;
            m_vertices[5].color = s_color2;
            m_vertices[6].color = s_color1;
            m_vertices[7].color = s_color1;
            break;
        }
    }
}

void ViperSegment::step(int32_t steps) {
    setTrackPoints(m_posBegin->traverse(-steps), m_posMiddle->traverse(-steps),
                   m_posEnd->traverse(-steps));
}

std::ostream& operator<<(std::ostream& os, const ViperSegment& vs) {
    os << "*** Segment (" << &vs << ") info ***" << std::endl;
    switch (vs.segmentType()) {
        case ViperSegment::SegmentType::Head:
            os << "Head, next segment is: " << vs.next() << std::endl;
            break;
        case ViperSegment::SegmentType::Body:
            os << "Body, previous segment was: " << vs.prev()
               << " and the next segment is: " << vs.next() << std::endl;
            break;
        case ViperSegment::SegmentType::Tail:
            os << "Tail, previous segment was: " << vs.prev() << std::endl;
            break;
    }
    os << "Segment starting at: " << vs.getPositionBegin()
       << ", going through: " << vs.getPositionMiddle()
       << ", ending at: " << vs.getPositionEnd() << std::endl;
    os << "*** End of segment info ***" << std::endl;
    return os;
}