#include "Track.hpp"

#include "debug.hpp"

TrackPoint::TrackPoint(sf::Vector2f v)
    : Vec2f(v), m_next(nullptr), m_prev(nullptr) {}

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
    auto tp = from;
    uint32_t n = 0;
    while (tp != to) {
        ++n;
        tp = tp->m_next;
    }
    return n;
}

float Track::length(TrackPoint const* from, TrackPoint const* to) const {
    auto t1 = from;
    auto t2 = from->next();
    float L = 0.f;
    while (t1 != t2) {
        L += distance(*t1, *t2);
        t1 = t2;
        t2 = t2->next();
    }
    return L;
}

void Track::pop_back() {
    if (empty())
        return;
    TrackPoint* delete_me = m_back;
    m_back = m_back->m_prev;
    m_back->m_next = nullptr;
    delete delete_me;
    --m_size;
}

void Track::pop_front() {
    if (empty())
        return;
    TrackPoint* delete_me = m_front;
    m_front = m_front->m_next;
    m_front->m_prev = nullptr;
    delete delete_me;
    --m_size;
}

void Track::create_back(Vec2f v) {
    push_back(new TrackPoint(v));
}

void Track::create_front(Vec2f v) {
    push_front(new TrackPoint(v));
}

void Track::push_back(TrackPoint* tp) {
    if (empty())
        m_front = m_back = tp;
    else {
        m_back->m_next = tp;
        tp->m_prev = m_back;
        m_back = tp;
        m_back->m_next = nullptr;
    }
    ++m_size;
}

void Track::push_front(TrackPoint* tp) {
    if (empty())
        m_front = m_back = tp;
    else {
        m_front->m_prev = tp;
        tp->m_next = m_front;
        m_front = tp;
        m_front->m_prev = nullptr;
    }
    ++m_size;
}