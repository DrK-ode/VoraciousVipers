#include <vvipers/Observer.hpp>

namespace VVipers {

void Observable::addObserver(Observer* observer) { m_observers.insert(observer); }

void Observable::removeObserver(Observer* observer) { m_observers.extract(observer); }

}  // namespace VVipers