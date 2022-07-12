#include <algorithm>
#include <vvipers/GameMaster.hpp>

namespace VVipers {

void GameMaster::onNotify(const GameEvent& event) {
    // If directly processable, do it!

    // Otherwise save it for later
    m_eventsToBeProcessed.push_back(event.clone());
}

void GameMaster::processEvents() {
    // Find all collision events
    auto collisions = filterEvents("Collision");
}

std::vector<const GameEvent*> GameMaster::filterEvents(
    std::string_view eventType) const {
    std::vector<const GameEvent*> filteredEvents;
    std::copy_if(m_eventsToBeProcessed.begin(), m_eventsToBeProcessed.end(),
                 filteredEvents.begin(), [&eventType](const GameEvent* event) {
                     return event->type() == eventType;
                 });
    return filteredEvents;
}

}  // namespace VVipers