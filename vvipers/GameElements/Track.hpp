#ifndef VVIPERS_GAMEELEMENTS_TRACK_HPP
#define VVIPERS_GAMEELEMENTS_TRACK_HPP

#include <vvipers/Utilities/debug.hpp>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>

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

    Vec2 gradient(const Time& t) const;
    double length() const;
    double length(const Time& from, const Time& to) const;
    Vec2 position(const Time& t) const;

    TrackPoint* front() const { return m_front; };
    TrackPoint* back() const { return m_back; };
    void pop_back();
    void pop_front();
    TrackPoint* create_point(const Vec2&, const Time&, TrackOrientation);
    TrackPoint* create_back(const Vec2& v, const Time& t) {
        return create_point(v, t, TrackOrientation::back);
    }
    TrackPoint* create_front(const Vec2& v, const Time& t) {
        return create_point(v, t, TrackOrientation::front);
    }

  private:
    const TrackPoint* at_or_earlier(const Time& t, const TrackPoint* starting_point = nullptr) const;
    void push_back(TrackPoint*);
    void push_front(TrackPoint*);
    size_t m_size;
    TrackPoint* m_front;
    TrackPoint* m_back;
};

std::ostream& operator<<(std::ostream& os, const Track& t);

class TrackPoint : public Vec2 {
  public:
    double distanceToEarlier() const { return m_distToEarlier; }
    TrackPoint* earlier() const { return m_earlier; }
    TrackPoint* later() const { return m_later; }
    void setEarlier(TrackPoint* tp);
    void setLater(TrackPoint* tp);
    // Traverses the track in any direction and returns the destination point
    TrackPoint* step(int32_t);
    Time getTime() const { return m_time; }
    void setTime(Time t) { m_time = t; }
    size_t stepsUntil(const TrackPoint* target) const;
    bool operator<(const TrackPoint& right) const {
        return m_time < right.m_time;
    }
    bool operator>(const TrackPoint& right) const {
        return m_time > right.m_time;
    }
    bool operator!=(const TrackPoint& right) const {
        return m_time != right.m_time or x != right.x or y != right.y;
    }
    bool operator==(const TrackPoint& right) const { return !(*this != right); }

  private:
    TrackPoint(const Vec2&, const Time&);
    Time m_time;
    TrackPoint* m_earlier;
    TrackPoint* m_later;
    double m_distToEarlier;

    friend TrackPoint* Track::create_point(const Vec2&, const Time&,
                                          TrackOrientation);
};

inline std::ostream& operator<<(std::ostream& os, const TrackPoint& t) {
    return os << Vec2(t) << ", t = " << t.getTime() << "s";
}

}  // namespace VVipers
#endif // VVIPERS_GAMEELEMENTS_TRACK_HPP
