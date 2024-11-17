#pragma once

#include <map>
#include <set>
#include <vvipers/GameElements/GameEvent.hpp>

namespace VVipers {

class Observer;

class Observable {
  public:
    /** The dtor will remove all observers **/
    virtual ~Observable();
    /** Will also update the observer's list of Observables **/
    void add_observer(Observer* observer,
                      const std::set<GameEvent::EventType>& eventTypes);
    /** Will also update the observer's list of Observables **/
    void remove_observer(Observer* observer);

  protected:
    void notify(const GameEvent* event) const;

  private:
    std::map<Observer*, std::set<GameEvent::EventType> > _observers;
};

class Observer {
  public:
    virtual ~Observer();
    /** The object event is pointing to is only guaranteed to exist at the time
     * of notification. If access to the event is needed later on it should be
     * cloned. **/
    virtual void on_notify(const GameEvent* event) = 0;

  private:
    /** The Observable will add itself to this set in order to be able to clean
     * up when an Observer or Observable is deleted. **/
    std::set<Observable*> _observing;
    friend void Observable::add_observer(Observer*,
                                         const std::set<GameEvent::EventType>&);
    friend void Observable::remove_observer(Observer*);
};

}  // namespace VVipers
