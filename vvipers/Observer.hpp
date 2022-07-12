#ifndef VVIPERS_OBSERVER_HPP
#define VVIPERS_OBSERVER_HPP

#include <set>
#include <vvipers/GameEvent.hpp>

namespace VVipers {

class Observer {
  public:
    virtual void onNotify(const GameEvent& event) = 0;
};

class Observable {
  public:
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);

  protected:
    void notify(const GameEvent& event) const {
        for (auto& observer : m_observers)
            observer->onNotify(event);
    }

  private:
    std::set<Observer*> m_observers;
};

}  // namespace VVipers

#endif  // VVIPERS_OBSERVER_HPP
