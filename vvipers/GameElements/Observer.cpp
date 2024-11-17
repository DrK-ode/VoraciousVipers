#include <vvipers/GameElements/Observer.hpp>

namespace VVipers {

Observable::~Observable() {
    /** The actual removal is only done by Observable::removeObserver() **/
    while( _observers.begin() != _observers.end() )
        remove_observer( _observers.begin()->first );
}

void Observable::add_observer(Observer* observer,
                             const std::set<GameEvent::EventType>& eventTypes) {
    _observers[observer] = eventTypes;
    observer->_observing.insert(this);
}

void Observable::remove_observer(Observer* observer) {
    _observers.erase(observer);
    observer->_observing.erase(this);
}

void Observable::notify(const GameEvent* event) const {
    for (auto& observer : _observers)
        if (observer.second.contains(event->type()))
            observer.first->on_notify(event);
}

Observer::~Observer() {
    /** The actual removal is only done by Observable::removeObserver() **/
    // m_observing will change during the while-loop
    while( _observing.size() > 0 )
        (*_observing.begin())->remove_observer(this);
}

}  // namespace VVipers