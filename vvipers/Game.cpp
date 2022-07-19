#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/Controller.hpp>
#include <vvipers/debug.hpp>
#include <vvipers/Game.hpp>
#include <vvipers/Level.hpp>
#include <vvipers/Player.hpp>
#include <vvipers/Viper.hpp>

namespace VVipers {

using namespace std::chrono_literals;

Game::Game() : m_exit(false) {
    Player* p1 = new Player("DefaultPlayerName");
    Viper* v1 = new Viper;
    v1->setup({400., 100.}, 0., 5s);
    this->addObserver(v1, {GameEvent::EventType::Update});
    m_collisionDetector.registerCollidable(v1);
    Controller* c1 = new ControllerGoingInCircles(45);
    c1->addObserver(this, {GameEvent::EventType::Steering});
    this->addObserver(c1, {GameEvent::EventType::Update});

    m_controllers.push_back(c1);
    m_players.push_back(p1);
    m_vipers.push_back(v1);
    connect(v1, p1);
    connect(c1, v1);

    Player* p2 = new Player("DefaultPlayerName");
    Viper* v2 = new Viper;
    v2->setup({400., 500.}, 0., 5s);
    m_collisionDetector.registerCollidable(v2);
    this->addObserver(v2, {GameEvent::EventType::Update});
    Controller* c2 = new KeyboardController(sf::Keyboard::A, sf::Keyboard::D);
    c2->addObserver(this, {GameEvent::EventType::Steering});
    this->addObserver(c2, {GameEvent::EventType::Update});

    m_controllers.push_back(c2);
    m_players.push_back(p2);
    m_vipers.push_back(v2);
    connect(v2, p2);
    connect(c2, v2);

    m_currentLevel = new Level("The first and only level");
    m_collisionDetector.registerCollidable(m_currentLevel);
    m_collisionDetector.addObserver(this, {GameEvent::EventType::Collision});
}

Game::~Game() {
    for (auto& p : m_players)
        delete p;
    for (auto& v : m_vipers)
        delete v;
    for (auto& c : m_controllers)
        delete c;
}

Viper* Game::belongsTo(const Controller* c) {
    if (m_mapControllerViper.contains(c))
        return m_mapControllerViper[c];
    else
        return nullptr;
}

Player* Game::belongsTo(const Viper* v) {
    if (m_mapViperPlayer.contains(v))
        return m_mapViperPlayer[v];
    else
        return nullptr;
}

void Game::connect(Controller* c, Viper* v) {
    m_mapControllerViper.insert({c, v});
}

void Game::connect(Viper* v, Player* p) { m_mapViperPlayer.insert({v, p}); }

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(*m_currentLevel, states);
    for (const auto& v : m_vipers)
        target.draw(*v, states);
}

void Game::onNotify(const GameEvent* event) {
    // If directly processable, do it!
    switch (event->type()) {
        case GameEvent::EventType::Keyboard: {
            switch (static_cast<const KeyboardEvent*>(event)->keyInfo.code) {
                // Check for game global keys
                case sf::Keyboard::Key::Escape: {
                    // Bring up menu
                    // But for now exit
                    signalExit();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case GameEvent::EventType::Window: {
            switch (static_cast<const WindowEvent*>(event)->eventType) {
                case sf::Event::EventType::Closed: {
                    signalExit();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default: {
            // Otherwise save it for later
            m_eventsToBeProcessed.insert(
                std::pair<GameEvent::EventType, GameEvent*>(event->type(),
                                                            event->clone()));
            break;
        }
    }
}

void Game::processEvents() {
    // Find all collision events
    auto [beginCollisionEvents, endCollisionEvent] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Collision);
    for (auto iter = beginCollisionEvents; iter != endCollisionEvent; ++iter) {
        const CollisionEvent* event = static_cast<const CollisionEvent*>(iter->second);
        logInfo( event->colliders );
    }
    auto [beginSteeringEvents, endSteeringEvent] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Steering);
    for (auto iter = beginSteeringEvents; iter != endSteeringEvent; ++iter) {
        // In the future different event might be combined and/or altered before
        // sending them to the viper.
        const SteeringEvent* steeringEvent =
            static_cast<const SteeringEvent*>(iter->second);
        Viper* viper = belongsTo(steeringEvent->controller);
        if (viper)
            viper->steer(steeringEvent);
    }
    // Delete fully processed events?
    // For now just delete all...
    for (auto& event : m_eventsToBeProcessed)
        delete event.second;
    m_eventsToBeProcessed.clear();
}

void Game::signalExit() {
    ApplicationEvent appEvent(ApplicationEvent::ApplicationEventType::Exit);
    notify(&appEvent);
    m_exit = true;
}

void Game::update(Time elapsedTime) {
    UpdateEvent updateEvent(elapsedTime);
    notify(&updateEvent);
    m_collisionDetector.checkForCollisions();
    processEvents();
}

}  // namespace VVipers