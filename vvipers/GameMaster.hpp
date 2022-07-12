#ifndef VVIPERS_GAMEMASTER_HPP
#define VVIPERS_GAMEMASTER_HPP

#include <string_view>
#include <vvipers/Observer.hpp>
#include <vector>

namespace VVipers{

// Some kind of event queue?
class GameMaster : public Observer, Observable {
    public:
    std::vector<const GameEvent*> filterEvents(const std::string_view eventType) const;
        // Choose to take immidiate action or to process it later
        void onNotify(const GameEvent& event) override;
        void processEvents();

    private:
        std::vector<const GameEvent*> m_eventsToBeProcessed;

};

}

#endif // VVIPERS_GAMEMASTER_HPP
