#ifndef VIPERSEGMENT_HPP
#define VIPERSEGMENT_HPP

#include <SFML/Graphics.hpp>

#include "Track.hpp"
#include "VectorMath.hpp"

class ViperSegment : public sf::Drawable {
  public:
    ViperSegment();
    ~ViperSegment();
    // Enum to describe what part the segment represents
    enum class SegmentType { Head, Body, Tail };
    // Must implement draw from Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void attach(ViperSegment*);  // Sets the other segment as the next segment
                                 // and updates both segment's ptrs
    TrackPoint* getPositionBegin() const { return m_posBegin; }
    TrackPoint* getPositionMiddle() const { return m_posMiddle; }
    TrackPoint* getPositionEnd() const { return m_posEnd; }
    ViperSegment* next() const;
    ViperSegment* prev() const;
    std::size_t distToTail() const;
    std::size_t distToHead() const;
    SegmentType segmentType() const;
    void setTrackPoints(TrackPoint* begin, TrackPoint* middle, TrackPoint* end);
    void step(int32_t);

  private:
    void updateVertices();
    static const float s_lengthWidthRatio;
    static const sf::Color s_color1;
    static const sf::Color s_color2;
    ViperSegment* m_next;
    ViperSegment* m_prev;
    TrackPoint* m_posBegin;
    TrackPoint* m_posMiddle;
    TrackPoint* m_posEnd;
    sf::VertexArray m_vertices;
};

std::ostream& operator<<(std::ostream& os, const ViperSegment& vs);

#endif