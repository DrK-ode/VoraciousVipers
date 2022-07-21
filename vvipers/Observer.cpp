#include <vvipers/Observer.hpp>

namespace VVipers {

Observable::~Observable() {
    /** The actual removal is only done by Observable::removeObserver() **/
    while( m_observers.begin() != m_observers.end() )
        removeObserver( m_observers.begin()->first );
}

void Observable::addObserver(Observer* observer,
                             const std::set<GameEvent::EventType>& eventTypes) {
    m_observers[observer] = eventTypes;
    observer->m_observing.insert(this);
}

void Observable::removeObserver(Observer* observer) {
    m_observers.erase(observer);
    observer->m_observing.erase(this);
}

void Observable::notify(const GameEvent* event) const {
    for (auto& observer : m_observers)
        if (observer.second.contains(event->type()))
            observer.first->onNotify(event);
}

Observer::~Observer() {
    /** The actual removal is only done by Observable::removeObserver() **/
    // m_observing will change during the while-loop
    while( m_observing.size() > 0 )
        (*m_observing.begin())->removeObserver(this);
}

}  // namespace VVipers