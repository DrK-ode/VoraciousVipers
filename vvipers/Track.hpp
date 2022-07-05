#ifndef VVIPERS_TRACK_HPP
#define VVIPERS_TRACK_HPP

#include <SFML/System/Time.hpp>
#include <deque>
#include <vvipers/VectorMath.hpp>

namespace VVipers {

enum class TrackOrientation { front, back };
class TrackPoint;

class Track {
  public:
    Track();
    void clear();
    bool empty() const { return !m_front; }
    size_t size() const { return m_size; }
    size_t size(TrackPoint const* from, TrackPoint const* to = nullptr) const;

    Vec2f direction( const sf::Time& t) const;
    float length() const;
    float length( const sf::Time& from, const sf::Time& to) const;
    Vec2f position( const sf::Time& t) const;

    TrackPoint* front() const { return m_front; };
    TrackPoint* back() const { return m_back; };
    void pop_back();
    void pop_front();
    TrackPoint* createPoint(const Vec2f&, const sf::Time&, TrackOrientation);
    TrackPoint* create_back(const Vec2f& v, const sf::Time& t) {
        return createPoint(v, t, TrackOrientation::back);
    }
    TrackPoint* create_front(const Vec2f& v, const sf::Time& t) {
        return createPoint(v, t, TrackOrientation::front);
    }

  private:
    
    void push_back(TrackPoint*);
    void push_front(TrackPoint*);
    size_t m_size;
    TrackPoint* m_front;
    TrackPoint* m_back;
};

std::ostream& operator<<(std::ostream& os, const Track& t);

class TrackPoint : public Vec2f {
  public:
  float distanceToNext() const {return m_distToNext;}
    TrackPoint* next() const { return m_next; }
    TrackPoint* prev() const { return m_prev; }
    void setNext(TrackPoint* tp);
    void setPrev(TrackPoint* tp);
    // Traverses the track in any direction and returns the destination point
    TrackPoint* step(int32_t);
    sf::Time getTime() const { return m_time; }
    void setTime(sf::Time t) { m_time = t; }
    size_t stepsUntil(const TrackPoint* target) const;
    bool operator<(const TrackPoint& right) const {
        return m_time < right.m_time;
    }
    bool operator>(const TrackPoint& right) const {
        return m_time > right.m_time;
    }
    bool operator!=(const TrackPoint& right) const {
        return m_time != right.m_time || x != right.x && y != right.y;
    }
    bool operator==(const TrackPoint& right) const { return !(*this != right); }

  private:
    TrackPoint(const Vec2f&, const sf::Time&);
    sf::Time m_time;
    TrackPoint* m_next;
    TrackPoint* m_prev;
    float m_distToNext;

    friend TrackPoint* Track::createPoint(const Vec2f&, const sf::Time&,
                                          TrackOrientation);
};

inline std::ostream& operator<<(std::ostream& os, const TrackPoint& t){
    return os << Vec2f(t) << ", t = " << t.getTime().asSeconds() << "s";
}

}  // namespace VVipers
#endif