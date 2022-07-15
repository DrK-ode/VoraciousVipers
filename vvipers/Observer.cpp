#include <vvipers/Observer.hpp>

namespace VVipers {

Observable::~Observable() {
    for (auto observer : m_observers)
        removeObserver(observer.first);
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
    // Need to iterate over a copy since the iterator might become invalidated
    auto clone = m_observing;
    for (auto observable : clone)
        observable->removeObserver(this);
}

}  // namespace VVipers