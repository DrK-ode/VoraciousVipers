#include <typeinfo>
#include <SFML/Graphics/RenderTarget.hpp>
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
    Viper* viper = new Viper(CID_type(GameObjects::Viper));
    viper->setup({400., 100.}, 0., 5s);
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

void Game::kill(Viper* viper) {
    auto player = findPlayerWith(viper);
    if( player )
        player->viper(nullptr);
    viper->state( Viper::ViperState::Dying );
}

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
    auto controller =
        addController(new KeyboardController(sf::Keyboard::A, sf::Keyboard::D));
    auto viper = addViper();
    auto player = addPlayer("Playername", controller, viper);

    m_currentLevel =
        new Level(CID_type(GameObjects::Level), "The first and only level");
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

void Game::processEvents() {
    // Find all collision events
    auto [beginCollisionEvents, endCollisionEvents] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Collision);
    for (auto iter = beginCollisionEvents; iter != endCollisionEvents; ++iter) {
        const CollisionEvent* event =
            static_cast<const CollisionEvent*>(iter->second);
        const CollisionTriplet& A = event->colliders.first;
        const CollisionTriplet& B = event->colliders.second;
        for (auto& ct : {A, B}) {
            if( ct.collidable->CID == CID_type(GameObjects::Viper) ) {
                Viper* viper = (Viper*)(ct.collidable);
                if (viper->state() == Viper::ViperState::Alive && ((ct.bodypart->BPID) &
                              BPID_type(Viper::ViperPart::Sensitive)))
                    kill(viper);}
        }
    }
    auto [beginDestroyEvents, endDestroyEvents] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Destroy);
    for (auto iter = beginDestroyEvents; iter != endDestroyEvents; ++iter){
        const DestroyMeEvent* destroyMeEvent =
            static_cast<const DestroyMeEvent*>(iter->second);
            if( typeid(*destroyMeEvent->objectPtr) == typeid(Viper) )
                deleteViper( (Viper*)(destroyMeEvent->objectPtr));
    }
    auto [beginSteeringEvents, endSteeringEvents] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Steering);
    for (auto iter = beginSteeringEvents; iter != endSteeringEvents; ++iter) {
        // In the future different event might be combined and/or altered before
        // sending them to the viper.
        const SteeringEvent* steeringEvent =
            static_cast<const SteeringEvent*>(iter->second);
        Player* player = findPlayerWith(steeringEvent->controller);
        if (!player) {
            tagError("Controller not attached to any player.");
            continue;
        }
        Viper* viper = player->viper();
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