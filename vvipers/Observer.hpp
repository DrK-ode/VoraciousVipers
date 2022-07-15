#ifndef VVIPERS_OBSERVER_HPP
#define VVIPERS_OBSERVER_HPP

#include <map>
#include <set>
#include <vvipers/GameEvent.hpp>

namespace VVipers {

class Observer {
  public:
    /** The object event is pointing to is only guaranteed to exist at the time
     * of notification. If access to the event is needed later on it should be
     * cloned. **/
    virtual void onNotify(const GameEvent* event) = 0;
};

class Observable {
  public:
    void addObserver(Observer* observer,
                     const std::set<GameEvent::EventType>& eventTypes);
    void removeObserver(Observer* observer);

  protected:
    void notify(const GameEvent* event) const;

  private:
    std::map<Observer*, std::set<GameEvent::EventType> > m_observers;
};

}  // namespace VVipers

#endif  // VVIPERS_OBSERVER_HPP
