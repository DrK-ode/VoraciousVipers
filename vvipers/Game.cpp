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

Game::Game(Vec2 windowSize)
    : sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y),
                       "VoraciousVipers"),
      m_exit(false) {
    // auto controllerM = addMouseController();
    // auto viperM = addViper();
    // auto playerM = addPlayer("PlayerM", controllerM, viperM);

    auto controllerK = addKeyboardController();
    auto viperK = addViper();
    auto playerK = addPlayer("PlayerK", controllerK, viperK);

    m_currentLevel = new Level("The first and only level", windowSize);
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
    viper->setup({400., 100.}, 0., 2.5);
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

Controller* Game::addMouseController() {
    setMouseCursorGrabbed(true);
    setMouseCursorVisible(false);
    sf::Mouse::setPosition(sf::Vector2i(getSize().x / 2, getSize().y / 2),
                           *this);
    return addController(new MouseController(this));
}

Controller* Game::addKeyboardController() {
    KeyboardController::KeyboardControls keys;
    keys.left = sf::Keyboard::A;
    keys.right = sf::Keyboard::D;
    keys.boost = sf::Keyboard::Space;
    return addController(new KeyboardController(keys));
}

void Game::draw() {
    clear(sf::Color::Black);
    RenderWindow::draw(*m_currentLevel);
    for (const auto& v : m_vipers)
        RenderWindow::draw(*v);
}

void Game::onNotify(const GameEvent* event) {
    // If directly processable, do it!
    switch (event->type()) {
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
    if (viper) {
        /* Boost if the steering event says so and either:
         *    1) Boost in inactive and the boost power is full
         *    2) Boost is active and the boost power is not depleted */

        // This allows the viper a turning radius equal to its width
        const double maxAngularSpeed = degPerRad * viper->turningRadius();
        double angularSpeed = event->turn * maxAngularSpeed;
        // Protect against erroneous input from controller
        if (std::abs(event->turn) > 1)
            angularSpeed /= std::abs(event->turn);
        double boost = 0.;
        if (event->boost &&
            ((viper->boost() > 0 and viper->boostCharge() > seconds(0)) or
             (viper->boost() == 0.0 and
              viper->boostCharge() == viper->boostMax())))
            boost = 1.;
        viper->steer(angularSpeed, boost);
    }
}

void Game::handleDestruction(const DestroyEvent* event) {
    if (typeid(*event->objectPtr) == typeid(Viper))
        // we could retrieve the non-const ptr from hte player but this is
        // easier
        deleteViper((Viper*)(event->objectPtr));
}

void Game::processWindowEvents() {
    sf::Event event;
    while (pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed: {
                signalExit();
                break;
            }
            case sf::Event::Resized: {
                break;
            }
            case sf::Event::KeyPressed: {
                switch (event.key.code) {
                    case sf::Keyboard::Escape: {
                        signalExit();
                        break;
                    }
                }
                break;
            }
        }
    }
}

void Game::processEvents() {
    processWindowEvents();
    // Find all collision events
    auto [beginCollisionEvents, endCollisionEvents] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Collision);
    for (auto iter = beginCollisionEvents; iter != endCollisionEvents; ++iter)
        handleCollisions(static_cast<const CollisionEvent*>(iter->second));
    auto [beginDestroyEvents, endDestroyEvents] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Destroy);
    for (auto iter = beginDestroyEvents; iter != endDestroyEvents; ++iter)
        handleDestruction(static_cast<const DestroyEvent*>(iter->second));
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
}

}  // namespace VVipers