#include <vvipers/Game.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;

Game::Game() {
    Player* p = new Player("DefaultPlayerName");
    p->setController(new ControllerGoingInCircles(45.));
    Viper* v = new Viper;
    v->setup({400., 100.}, 0., 5s);
    m_players[p] = v;
}

Game::~Game() {
    for (const auto& p : m_players) {
        delete p.first;
        delete p.second;
    }
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& p : m_players)
        target.draw(*p.second, states);
}

void Game::update(Time elapsedTime) {
    for(const auto& p : m_players) {
        p.first->getController()->onUpdate(elapsedTime, *p.second);
        p.second->update(elapsedTime);
    }
}

}  // namespace VVipers