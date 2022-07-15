#include <vvipers/Observer.hpp>

namespace VVipers {

void Observable::addObserver(Observer* observer,
                             const std::set<GameEvent::EventType>& eventTypes) {
    m_observers[observer] = eventTypes;
}

void Observable::removeObserver(Observer* observer) {
    m_observers.erase(observer);
}

void Observable::notify(const GameEvent* event) const {
    for (auto& observer : m_observers)
        if (observer.second.contains(event->type()))
            observer.first->onNotify(event);
}

}  // namespace VVipers