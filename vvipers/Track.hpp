#ifndef VVIPERS_TRACK_HPP
#define VVIPERS_TRACK_HPP

#include <deque>

#include <vvipers/VectorMath.hpp>

class Track;

class TrackPoint : public Vec2f {
  public:
    TrackPoint* next() const { return m_next; };
    TrackPoint* prev() const { return m_prev; };
    // Traverses the track in any direction and returns the destination point
    TrackPoint* step(int32_t);
    size_t stepsUntil(const TrackPoint* target) const;

  private:
    TrackPoint(sf::Vector2f);
    TrackPoint* m_next;
    TrackPoint* m_prev;

    friend Track;
};

class Track {
  public:
    Track();
    void clear();
    bool empty() const { return !m_front; }
    size_t size() const { return m_size; }
    size_t size(TrackPoint const* from, TrackPoint const* to = nullptr) const;
    float length() const {return length(m_front,m_back);};
    float length(TrackPoint const* from, TrackPoint const* to) const;
    TrackPoint* front() const { return m_front; };
    TrackPoint* back() const { return m_back; };
    void pop_back();
    void pop_front();
    void create_back(Vec2f);
    void create_front(Vec2f);

  private:
    void push_back(TrackPoint*);
    void push_front(TrackPoint*);
    size_t m_size;
    TrackPoint* m_front;
    TrackPoint* m_back;
};

#endif