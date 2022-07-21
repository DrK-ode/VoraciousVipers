#include <SFML/Graphics/RenderTarget.hpp>
#include <typeinfo>
#include <vvipers/Bodypart.hpp>
#include <vvipers/Controller.hpp>
#include <vvipers/Game.hpp>
#include <vvipers/Level.hpp>
#include <vvipers/Player.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;

Controller* Game::addController(Controller* controller) {
    controller->addObserver(this, {GameEvent::EventType::Steering});
    this->addObserver(controller, {GameEvent::EventType::Update});
    m_controllers.insert(controller);
    return controller;
}

void Game::deleteController(Controller* controller) {
    m_controllers.erase(controller);
    delete controller;
}

Player* Game::addPlayer(const std::string& name, Controller* controller,
                        Viper* viper) {
    Player* player = new Player(name, controller, viper);
    m_players.insert(player);
    return player;
}

void Game::deletePlayer(Player* player) {
    m_players.erase(player);
    delete player;
}

Viper* Game::addViper(/* startConditions? */) {
    Viper* viper = new Viper();
    viper->setup({400., 100.}, 0., 20s);
    this->addObserver(viper, {GameEvent::EventType::Update});
    viper->addObserver(this, {GameEvent::EventType::Destroy});
    m_collisionDetector.registerCollidable(viper);
    m_vipers.insert(viper);
    return viper;
}

void Game::deleteViper(Viper* viper) {
    m_collisionDetector.deRegisterCollidable(viper);
    m_vipers.erase(viper);
    delete viper;
}

void Game::killViper(Viper* viper) { viper->state(Viper::ViperDying); }

Player* Game::findPlayerWith(const Controller* controller) const {
    for (auto player : m_players)
        if (player->controller() == controller)
            return player;
    return nullptr;
}

Player* Game::findPlayerWith(const Viper* viper) const {
    for (auto player : m_players)
        if (player->viper() == viper)
            return player;
    return nullptr;
}

Game::Game() : m_exit(false) {
    KeyboardController::KeyboardControls keys;
    keys.left = sf::Keyboard::A;
    keys.right = sf::Keyboard::D;
    keys.boost = sf::Keyboard::Space;
    auto controller = addController(new KeyboardController(keys));
    auto viper = addViper();
    auto player = addPlayer("Playername", controller, viper);

    m_currentLevel = new Level("The first and only level");
    m_collisionDetector.registerCollidable(m_currentLevel);
    m_collisionDetector.addObserver(this, {GameEvent::EventType::Collision});
}

Game::~Game() {
    for (auto p : m_players)
        delete p;
    for (auto v : m_vipers)
        delete v;
    for (auto c : m_controllers)
        delete c;
}

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

void Game::handleCollisions(const CollisionEvent* event) {
    const CollisionTriplet& A = event->colliders.first;
    const CollisionTriplet& B = event->colliders.second;
    for (auto& ct : {A, B}) {
        if (typeid(*ct.collidable) == typeid(Viper)) {
            Viper* viper = (Viper*)(ct.collidable);
            if (viper->state() == Viper::ViperAlive &&
                ((ct.bodypart->partID) & Viper::ViperSensitivePart))
                killViper(viper);
        }
    }
}

void Game::handleSteering(const SteeringEvent* event) {
    Player* player = findPlayerWith(event->controller);
    if (!player) {
        tagError("Controller not attached to any player.");
        return;
    }
    Viper* viper = player->viper();
    if (viper)
        viper->steer(event);
}

void Game::handleDestruction( const DestroyEvent* event ){
        if (typeid(*event->objectPtr) == typeid(Viper))
        // we could retrieve the non-const ptr from hte player but this is easier
            deleteViper((Viper*)(event->objectPtr));
}

void Game::processEvents() {
    // Find all collision events
    auto [beginCollisionEvents, endCollisionEvents] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Collision);
    for (auto iter = beginCollisionEvents; iter != endCollisionEvents; ++iter)
        handleCollisions(static_cast<const CollisionEvent*>(iter->second));
    auto [beginDestroyEvents, endDestroyEvents] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Destroy);
    for (auto iter = beginDestroyEvents; iter != endDestroyEvents; ++iter)
        handleDestruction(
            static_cast<const DestroyEvent*>(iter->second));
    auto [beginSteeringEvents, endSteeringEvents] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Steering);
    for (auto iter = beginSteeringEvents; iter != endSteeringEvents; ++iter)
        handleSteering(static_cast<const SteeringEvent*>(iter->second));
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