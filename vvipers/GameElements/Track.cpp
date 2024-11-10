#include <algorithm>
#include <deque>
#include <iterator>
#include <sstream>
#include <vvipers/GameElements/Track.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/Utilities/Time.hpp"

namespace VVipers {

TemporalTrack::TemporalTrack(const Vec2& p1, const Time& t1, const Vec2& p2,
                             const Time& t2) {
    if (t1 <= t2)
        throw std::runtime_error(
            "Trying to initiate a temporal track out of temporal order.");
    m_points.emplace(m_points.cend(), p1, t1, Vec2(), timeFromSeconds(0.));
    m_points.emplace(m_points.cend(), p2, t2,
                     (p1 - p2) / timeAsSeconds(t1 - t2), t1 - t2);
}

void TemporalTrack::create_back(const Vec2& v, const Time& delta) {
    if (delta <= timeFromSeconds(0.))
        throw std::runtime_error(
            "Trying to create a temporal track point out of temporal "
            "order.");
    m_points.emplace(m_points.cend(), tail() + v * timeAsSeconds(delta),
                     tail().spawn_time - delta, v, delta);
}

void TemporalTrack::create_front(const Vec2& v, const Time& delta) {
    if (delta <= timeFromSeconds(0.))
        throw std::runtime_error(
            "Trying to create a temporal track point out of temporal "
            "order.");
    m_points.front().delta_t = delta;
    m_points.front().velocity = v;
    m_points.emplace(m_points.cbegin(), head() + v * timeAsSeconds(delta),
                     head().spawn_time + delta, v, timeFromSeconds(0.));
}

Vec2 TemporalTrack::velocity(const Time& t) const {
    if (m_points.size() < 2) {
        tagError("Cannot compute a direction with < 2 TrackPoints.");
        throw std::runtime_error(
            "Cannot compute a direction with < 2 TrackPoints.");
    }

    auto tp = at_or_before(t);
    if (tp == m_points.cend())
        tp = std::prev(tp);
    return tp->velocity;
}

double TemporalTrack::length() const {
    double length = 0.;
    for (auto iter = m_points.cbegin(); iter != m_points.cend(); ++iter) {
        length += iter->velocity.abs() * timeAsSeconds(iter->delta_t);
    }
    return length;
}

double TemporalTrack::length(const Time& t1, const Time& t2) const {
    if (t1 < t2) {
        std::stringstream msg;
        msg << "Requesting length between t1 = " << t1 << " and t2 = " << t2
            << ", which means going backwards in time.";
        tagError(msg.str());
        throw std::runtime_error(msg.str());
    }
    if (t1 > m_points.front().spawn_time || t2 < m_points.back().spawn_time) {
        std::stringstream msg;
        msg << "Requesting length between t1 = " << t1 << " and t2 = " << t2
            << ", which is outside viper time interval("
            << m_points.back().spawn_time << " - "
            << m_points.front().spawn_time << ").";
        tagError(msg.str());
        throw std::runtime_error(msg.str());
    }
    if (t1 == t2)
        return 0;

    double length = 0.;
    // p1 is guaranteed to exist and spawned after t2
    auto p1 = at_or_later(t1);
    // p2 is guaranteed to exist and spawned before t1
    auto p2 = at_or_before(t2);
    for (auto iter = p2; iter != p1; --iter) {
        length += iter->velocity.abs() * timeAsSeconds(iter->delta_t);
    }
    auto tmp1 = std::next(p1);
    length -= tmp1->velocity.abs() * timeAsSeconds(p1->spawn_time - t1);
    length -= p2->velocity.abs() * timeAsSeconds(t2 - p2->spawn_time);

    return length;
}

Vec2 TemporalTrack::position(const Time& t) const {
    auto p = at_or_before(t);
    if (p == m_points.cend())
        p = t > head().spawn_time ? m_points.cbegin() : std::prev(p);

    return *p + p->velocity * timeAsSeconds(t - p->spawn_time);
}

// Find TemporalTrackPoint with spawn_time <= t
tt_const_iter TemporalTrack::at_or_before(const Time& t,
                                          const tt_const_iter& start_iter,
                                          const tt_const_iter& end_iter) const {
    return std::lower_bound(start_iter, end_iter, t,
                            [](const TemporalTrackPoint& tp, const Time& t) {
                                return tp.spawn_time > t;
                            });
}

// Find TemporalTrackPoint with spawn_time >= t
tt_const_iter TemporalTrack::at_or_later(const Time& t,
                                         const tt_const_iter& start_iter,
                                         const tt_const_iter& end_iter) const {
    tt_const_iter iter =
        std::upper_bound(start_iter, end_iter, t,
                         [](const Time& t, const TemporalTrackPoint& tp) {
                             return tp.spawn_time < t;
                         });
    /* Upper bound returns a TemporalTrackPoint with a spawn_time < t.
     * If that is the first iterator in the interval there is no element with
     * spawn_time >= t. If that is the end_iter the sought element is either the
     * previous one or there is no suitable element within the interval. */
    if (iter == start_iter)
        return m_points.cend();
    iter = std::prev(iter);
    if (iter->spawn_time < t)
        return m_points.cend();
    return iter;
}

void TemporalTrack::pop_back() {
    if (m_points.size() <= 2)
        throw std::runtime_error(
            "Trying to reduce temporal track length to less than 2.");
    return m_points.pop_back();
}

void TemporalTrack::pop_front() {
    if (m_points.size() <= 2)
        throw std::runtime_error(
            "Trying to reduce temporal track length to less than 2.");
    return m_points.pop_front();
}

void TemporalTrack::remove_trailing(const Time& t) {
    // Never remove the first to points
    auto first_to_erase =
        at_or_before(t, m_points.cbegin() + 2, m_points.cend());
    m_points.erase(first_to_erase, m_points.cend());
}

std::ostream& operator<<(std::ostream& os, const TemporalTrack& t) {
    os << "Track size: " << t.size() << ", track length: " << t.length()
       << std::endl;
    return os;
}

}  // namespace VVipers