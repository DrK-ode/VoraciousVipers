#include <sstream>
#include <vvipers/GameElements/Track.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

TrackPoint::TrackPoint(const Vec2& v, const Time& t)
    : Vec2(v), m_time(t), m_next(nullptr), m_prev(nullptr), m_distToNext(0) {}

void TrackPoint::setNext(TrackPoint* tp) {
    m_next = tp;
    m_distToNext = 0;
    if (tp) {
        tp->m_prev = this;
        m_distToNext = distance(*this, *tp);
    }
}
void TrackPoint::setPrev(TrackPoint* tp) {
    m_prev = tp;
    if (tp) {
        tp->m_next = this;
        tp->m_distToNext = distance(*tp, *this);
    }
}

TrackPoint* TrackPoint::step(int32_t s) {
    if (s > 0) {
        if (!m_next) {
            logWarning("No next TrackPoint.");
            throw std::out_of_range(
                "Trying to access TrackPoint beyond the Track end.");
        }
        return m_next->step(s - 1);
    } else if (s < 0) {
        if (!m_prev) {
            logWarning("No previous TrackPoint.");
            throw std::out_of_range(
                "Trying to access TrackPoint beyond the Track end.");
        }
        return m_prev->step(s + 1);
    } else
        return this;
}

size_t TrackPoint::stepsUntil(const TrackPoint* target) const {
    auto tp = this;
    uint32_t n = 0;
    while (tp != target) {
        ++n;
        tp = tp->m_next;
    }
    return n;
}

Track::Track() : m_size(0), m_front(nullptr), m_back(nullptr) {}

void Track::clear() {
    TrackPoint* tp;
    while (m_front) {
        tp = m_front->next();
        delete m_front;
        m_front = tp;
    }
    m_back = nullptr;
}

size_t Track::size(TrackPoint const* from, TrackPoint const* to) const {
    if (!from)
        return 0;
    if (from == m_front && to == m_back)
        return m_size;
    return from->stepsUntil(to);
}

Vec2 Track::gradient(const Time& t) const {
    if (m_size < 2) {
        tagError("Cannot compute direction with < 2 TrackPoints.");
        throw std::runtime_error(
            "Cannot compute direction with < 2 TrackPoints.");
    }

    TrackPoint* p2 = m_front->next();
    while (p2 && p2->getTime() > t)
        p2 = p2->next();
    if (!p2)  // t < time at end of track
        p2 = m_back;
    else if (p2 == m_front)  // t >= time at start of track
        p2 = m_front->next();
    TrackPoint* p1 = p2->prev();
    return (*p1 - *p2) / timeAsSeconds(p1->getTime() - p2->getTime());
}

double Track::length() const {
    if (!m_front)
        return 0;
    return length(m_front->getTime(), m_back->getTime());
}

double Track::length(const Time& t1, const Time& t2) const {
    // Typically the front of the track is the youngest so we expect t1 > t2
    // But if t2 > t1 we return a negative length
    if (t1 < t2)
        return -length(t2, t1);
    // t1 or t2 outside the track's range
    if (t1 > m_front->getTime() || t2 < m_back->getTime()) {
        std::stringstream msg;
        msg << "Requesting length between t1 = " << t1 << " and t2 = " << t2
            << ", which is outside viper time interval(" << m_back->getTime()
            << " - " << m_front->getTime() << ").";
        tagError(msg.str());
        throw std::runtime_error(msg.str());
    }
    // Or if a zero time interval has been passed to the method
    if (t1 == t2)
        return 0;

    double L = 0.f;
    // Find first track point
    TrackPoint* p2 = m_front;
    while (p2 && p2->getTime() > t1)
        p2 = p2->next();
    // Linear interpolation of the distance to the next TrackPoint
    TrackPoint* p1 = p2->prev();
    if (p1)  // Otherwise p2 == m_front and no distance needs to be added
        L += p1->distanceToNext() *
             ((p2->getTime() - t1) / (p2->getTime() - p1->getTime()));
    // Add all distances until t2 has been passed
    while (p2 && p2->getTime() > t2) {
        L += p2->distanceToNext();
        p2 = p2->next();
    }
    if (!p2)  // p1 == m_back
        return L;
    // The full distance between p1 and p2 was added to L, this must be
    // corrected for.
    p1 = p2->prev();
    L -= p1->distanceToNext() *
         ((p2->getTime() - t2) / (p2->getTime() - p1->getTime()));

    return L;
}

Vec2 Track::position(const Time& t) const {
    TrackPoint* p2 = m_front;
    while (p2 && p2->getTime() > t)
        p2 = p2->next();
    if (!p2)  // t < time at end of track
        p2 = m_back;
    else if (p2 == m_front)  // t >= time at start of track
        p2 = m_front->next();

    TrackPoint* p1 = p2->prev();
    // The sought position lies between p1 and p2, linear interpolation (or
    // extrapolation)
    return *p1 + (*p2 - *p1) *
                     ((t - p1->getTime()) / (p2->getTime() - p1->getTime()));
}

void Track::pop_back() {
    if (empty())
        return;
    TrackPoint* delete_me = m_back;
    m_back = m_back->prev();
    m_back->setNext(nullptr);
    delete delete_me;
    --m_size;
}

void Track::pop_front() {
    if (empty())
        return;
    TrackPoint* delete_me = m_front;
    m_front = m_front->next();
    m_front->setPrev(nullptr);
    delete delete_me;
    --m_size;
}

TrackPoint* Track::createPoint(const Vec2& v, const Time& t,
                               TrackOrientation ori) {
    TrackPoint* p = new TrackPoint(v, t);
    switch (ori) {
        case TrackOrientation::front:
            push_front(p);
            break;
        case TrackOrientation::back:
            push_back(p);
            break;
    }
    return p;
}

void Track::push_back(TrackPoint* tp) {
    if (empty())
        m_front = m_back = tp;
    else {
        if (tp->getTime() > m_back->getTime())
            throw std::runtime_error(
                "Trying to push back a track point out of temporal order.");
        m_back->setNext(tp);
        tp->setPrev(m_back);
        tp->setNext(nullptr);
        m_back = tp;
    }
    ++m_size;
}

void Track::push_front(TrackPoint* tp) {
    if (empty())
        m_front = m_back = tp;
    else {
        if (tp->getTime() < m_front->getTime())
            throw std::runtime_error(
                "Trying to push front a track point out of temporal order.");
        m_front->setPrev(tp);
        tp->setNext(m_front);
        tp->setPrev(nullptr);
        m_front = tp;
    }
    ++m_size;
}

std::ostream& operator<<(std::ostream& os, const Track& t) {
    os << "Track size: " << t.size() << ", track length: " << t.length()
       << std::endl;
    TrackPoint* p = t.front();
    int i = 0;
    while (p) {
        os << "  Trackpoint " << i++ << ": " << *p << std::endl;
        p = p->next();
    }
    return os;
}

}  // namespace VVipers