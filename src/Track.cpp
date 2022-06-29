#include "Track.hpp"

#include "debug.hpp"

TrackPoint::TrackPoint(sf::Vector2f v, float angle)
    : Vec2f(v), m_angle(angle), m_next(nullptr), m_prev(nullptr) {}

TrackPoint* TrackPoint::traverse(int32_t s) {
    if (s > 0) {
        if (!m_next) {
            logWarning("No next TrackPoint.");
            throw std::out_of_range(
                "Trying to access TrackPoint beyond the Track end.");
        }
        return m_next->traverse(s - 1);
    } else if (s < 0) {
        if (!m_prev) {
            logWarning("No previous TrackPoint.");
            throw std::out_of_range(
                "Trying to access TrackPoint beyond the Track end.");
        }
        return m_prev->traverse(s + 1);
    } else
        return this;
}

void Track::clear() { m_trackPoints.clear(); }

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
    delete m_trackPoints.back();
    m_trackPoints.pop_back();
    m_trackPoints.back()->m_next = nullptr;
}

void Track::pop_front() {
    delete m_trackPoints.front();
    m_trackPoints.pop_front();
    m_trackPoints.front()->m_next = nullptr;
}

void Track::create_back(Vec2f v, float angle) {
    TrackPoint* secondToLast = m_trackPoints.empty() ? nullptr : m_trackPoints.back();
    TrackPoint* last = new TrackPoint(v,angle);
    m_trackPoints.push_back(last);
    if (secondToLast) {
        last->m_prev = secondToLast;
        secondToLast->m_next = last;
    }
}

void Track::create_front(Vec2f v, float angle) {
    TrackPoint* second = m_trackPoints.empty() ? nullptr : m_trackPoints.front();
    TrackPoint* first = new TrackPoint(v);
    m_trackPoints.push_front(first);
    if (second) {
        second->m_prev = first;
        first->m_next = second;
    }
}