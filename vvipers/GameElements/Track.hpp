#pragma once

#include <pthread.h>

#include <cstddef>
#include <deque>
#include <vvipers/Utilities/Time.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

class TemporalTrackPoint : public Vec2 {
  public:
    TemporalTrackPoint(const Vec2& pos, const Time& t, const Vec2& v,
                       const Time& delta)
        : Vec2(pos), spawn_time(t), velocity(v), delta_t(delta) {}
    bool operator!=(const TemporalTrackPoint& right) const {
        return !(this->operator==(right));
    }
    bool operator==(const TemporalTrackPoint& right) const {
        return this->spawn_time == right.spawn_time && this->x == right.x &&
               this->y == right.y && this->velocity == right.velocity;
    }

    Time spawn_time;
    Vec2 velocity;
    Time delta_t;
};

inline std::ostream& operator<<(std::ostream& os, const TemporalTrackPoint& p) {
    return os << Vec2(p) << ", t = " << p.spawn_time << ", v = " << p.velocity
              << ", delta_t = " << p.delta_t;
}

typedef std::deque<TemporalTrackPoint>::const_iterator tt_const_iter;

/// The TemporalTrack promises to allways have at least two TemporalTrackPoints.
/// Therefore, the constructor needs two initial points and any method of
/// removing points must make sure to not remove too many.
class TemporalTrack {
  public:
    TemporalTrack(const Vec2& p1, const Time& t1, const Vec2& p2,
                  const Time& t2);
    size_t size() const { return m_points.size(); }
    double length() const;
    double length(const Time& from, const Time& to) const;

    tt_const_iter begin() const { return m_points.cbegin(); }
    tt_const_iter end() const { return m_points.cend(); }
    const TemporalTrackPoint& head() const { return m_points.front(); }
    const TemporalTrackPoint& tail() const { return m_points.back(); }
    const Time& head_time() const { return m_points.front().spawn_time; }
    const Time& tail_time() const { return m_points.back().spawn_time; }
    const Vec2& head_position() const { return m_points.front(); }
    const Vec2& tail_position() const { return m_points.back(); }
    Vec2 position(const Time& t) const;
    Vec2 velocity(const Time& t) const;

    void create_back(const Vec2& v, const Time& t);
    void create_front(const Vec2& v, const Time& t);
    // Will always leave a minimum of two points.
    void remove_trailing(const Time& t);

    // Will always leave a minimum of two points.
    void pop_back();
    // Will always leave a minimum of two points.
    void pop_front();

    tt_const_iter at_or_before(const Time& t, const tt_const_iter& start,
                               const tt_const_iter& end) const;
    tt_const_iter at_or_later(const Time& t, const tt_const_iter& start,
                              const tt_const_iter& end) const;
    tt_const_iter at_or_before(const Time& t) const {
        return at_or_before(t, m_points.cbegin(), m_points.cend());
    }
    tt_const_iter at_or_later(const Time& t) const {
        return at_or_later(t, m_points.cbegin(), m_points.end());
    }

  private:
    std::deque<TemporalTrackPoint> m_points;
};

std::ostream& operator<<(std::ostream& os, const TemporalTrack& t);

}  // namespace VVipers
