#include <vvipers/Game.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

Game::Game() {
    Player* p = new Player("DefaultPlayerName");
    p->setController(new ControllerGoingInCircles(0.5f));
    Viper* v = new Viper;
    v->setup({400.f, 100.f}, 0.f, sf::seconds(2));
    m_players[p] = v;
}

Game::~Game() {
    for_each(m_players.begin(), m_players.end(), [](auto p) {
        delete p.first;
        delete p.second;
    });
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for_each(m_players.begin(), m_players.end(),
             [&](auto p) { target.draw(*p.second, states); });
}

void Game::tick(sf::Time elapsedTime) {
    for_each(m_players.begin(), m_players.end(), [&](auto p) {
        p.first->getController()->onTick(elapsedTime,*p.second);
        p.second->tick(elapsedTime);
    });
}

}  // namespace VVipers