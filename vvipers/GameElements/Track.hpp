#ifndef VVIPERS_GAMEELEMENTS_TRACK_HPP
#define VVIPERS_GAMEELEMENTS_TRACK_HPP

#include <cstddef>
#include <deque>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

class TemporalTrackPoint : public Vec2 {
  public:
    TemporalTrackPoint(const Vec2& pos, const Time& t, double d)
        : Vec2(pos), spawn_time(t), distance_from_previous_point(d) {}
    bool operator!=(const TemporalTrackPoint& right) const {
        return spawn_time != right.spawn_time or x != right.x or y != right.y;
    }
    bool operator==(const TemporalTrackPoint& right) const {
        return !(*this != right);
    }

    Time spawn_time;
    double distance_from_previous_point;
};

inline std::ostream& operator<<(std::ostream& os, const TemporalTrackPoint& p) {
    return os << Vec2(p) << ", t = " << p.spawn_time << ", delta L = " << p.distance_from_previous_point;
}

class TemporalTrack {
  public:
    TemporalTrack(const Vec2& p1, const Time& t1, const Vec2& p2,
                  const Time& t2) {
        create_back(p1, t1);
        create_back(p2, t2);
    }

    size_t size() const { return m_points.size(); }
    double length() const;
    double length(const Time& from, const Time& to) const;

    const Time& head_time() const { return m_points.front().spawn_time; }
    const Time& tail_time() const { return m_points.back().spawn_time; }
    const Vec2& head_position() const { return m_points.front(); }
    const Vec2& tail_position() const { return m_points.back(); }
    Vec2 position(const Time& t) const;
    Vec2 gradient(const Time& t) const;

    void create_back(const Vec2& v, const Time& t);
    void create_front(const Vec2& v, const Time& t);
    void remove_trailing(const Time& t);

    void pop_back();
    void pop_front();

  private:
    const std::deque<TemporalTrackPoint>::const_iterator at_or_before(
        const Time& t,
        const std::deque<TemporalTrackPoint>::const_iterator) const;
    const std::deque<TemporalTrackPoint>::const_iterator at_or_before(
        const Time& t) const {
        return at_or_before(t, m_points.cbegin());
    }
    const std::deque<TemporalTrackPoint>::const_iterator at_or_later(
        const Time& t,
        const std::deque<TemporalTrackPoint>::const_iterator) const;
    const std::deque<TemporalTrackPoint>::const_iterator at_or_later(
        const Time& t) const {
        return at_or_later(t, m_points.end());
    }

    std::deque<TemporalTrackPoint> m_points;
};

std::ostream& operator<<(std::ostream& os, const TemporalTrack& t);

}  // namespace VVipers
#endif  // VVIPERS_GAMEELEMENTS_TRACK_HPP
