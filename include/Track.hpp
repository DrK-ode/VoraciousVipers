#ifndef TRACK_HPP
#define TRACK_HPP

#include <deque>

#include "VectorMath.hpp"

class Track;

class TrackPoint : public Vec2f {
  public:
    TrackPoint* next() const { return m_next; };
    TrackPoint* prev() const { return m_prev; };
    // Traverses the track in any direction and returns the destination point
    TrackPoint* traverse(int32_t); 

  private:
    TrackPoint(sf::Vector2f, float angle = 0.f);
    float m_angle;
    TrackPoint* m_next;
    TrackPoint* m_prev;

    friend Track;
};

class Track {
  public:
    void clear();
    float length() const { return length(front(), back()); }
    float length(TrackPoint const* from, TrackPoint const* to) const;
    TrackPoint* front() const { return m_trackPoints.front(); };
    TrackPoint* back() const { return m_trackPoints.back(); };
    void pop_back();
    void pop_front();
    void create_back(Vec2f, float angle);
    void create_front(Vec2f, float angle);

  private:
    std::deque<TrackPoint*> m_trackPoints;
};

#endif