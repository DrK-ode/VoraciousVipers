#include <vvipers/Game.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;

Game::Game() {
    for (int i = 0; i < 2; ++i) {
        Player* p = new Player("DefaultPlayerName");
        Viper* v = new Viper;
        Controller* c = new ControllerGoingInCircles(45. - 75*i);
        m_controllers.push_back(c);
        m_players.push_back(p);
        m_vipers.push_back(v);

        v->setController(c);
        v->setup({400., 100.+300*i}, 0.-90*i, 5s);
        m_collisionDetector.registerCollidable(&v->getPhysicalViper());
    }
    m_currentLevel = new Level("The first and only level");
    m_collisionDetector.registerCollidable(m_currentLevel);
    m_collisionDetector.addObserver(&m_GM);
}

Game::~Game() {
    for (auto& p : m_players)
        delete p;
    for (auto& v : m_vipers)
        delete v;
    // Important to destroy controller after viper
    for (auto& c : m_controllers)
        delete c;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw( *m_currentLevel, states );
    for (const auto& v : m_vipers)
        target.draw(*v, states);
}

void Game::update(Time elapsedTime) {
    for (auto& c : m_controllers) {
        c->onUpdate(elapsedTime);
    }
    for (auto& v : m_vipers) {
        v->update(elapsedTime);
    }
}

}  // namespace VVipers