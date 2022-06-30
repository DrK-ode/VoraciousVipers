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
    TrackPoint* step(int32_t);

  private:
    TrackPoint(sf::Vector2f, float angle = 0.f);
    float m_angle;
    TrackPoint* m_next;
    TrackPoint* m_prev;

    friend Track;
};

class Track {
  public:
    Track();
    void clear();
    bool empty() const { return !m_front; }
    float size() const { return size(m_front, m_back); }
    float size(TrackPoint const* from, TrackPoint const* to) const;
    TrackPoint* front() const { return m_front; };
    TrackPoint* back() const { return m_back; };
    void pop_back();
    void pop_front();
    void create_back(Vec2f, float angle);
    void create_front(Vec2f, float angle);

  private:
    void push_back(TrackPoint*);
    void push_front(TrackPoint*);
    TrackPoint* m_front;
    TrackPoint* m_back;
};

#endif