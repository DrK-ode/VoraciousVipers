#include <SFML/Graphics/RenderTarget.hpp>
#include <typeinfo>
#include <vvipers/Bodypart.hpp>
#include <vvipers/Controller.hpp>
#include <vvipers/Game.hpp>
#include <vvipers/Player.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/Walls.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;

Game::Game(Vec2 windowSize)
    : sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y),
                       "VoraciousVipers"),
      m_exit(false) {
    const sf::Vector2f statusBarRelSize(1, 0.1);
    const sf::Vector2f statusBarSize(getSize().x * statusBarRelSize.x,
                                     getSize().y * statusBarRelSize.y);
    m_statusBarView = new sf::View();
    m_statusBarView->setSize(statusBarSize);
    m_statusBarView->setCenter(statusBarSize / 2);
    m_statusBarView->setViewport(sf::FloatRect({0.f, 0.f}, statusBarRelSize));

    const sf::Vector2f gameRelSize(1, 1 - statusBarRelSize.y);
    const sf::Vector2f gameSize(getSize().x * gameRelSize.x,
                                getSize().y * gameRelSize.y);
    m_gameView = new sf::View();
    m_gameView->setSize(gameSize);
    m_gameView->setCenter(gameSize / 2);
    m_gameView->setViewport(
        sf::FloatRect({0.f, statusBarRelSize.y}, gameRelSize));

    // auto controllerM = addMouseController();
    // auto viperM = addViper();
    // auto playerM = addPlayer("PlayerM", controllerM, viperM);

    m_walls = new Walls(gameSize);
    m_collisionDetector.registerCollidable(m_walls);

    auto controllerK = addKeyboardController();
    auto viperK = addViper();
    auto playerK = addPlayer("PlayerK", controllerK, viperK);
    playerK->color(sf::Color::Red);
}

Game::~Game() {
    for (auto p : m_players)
        delete p;
    for (auto v : m_vipers)
        delete v;
    for (auto c : m_controllers)
        delete c;
    delete m_statusBarView;
    delete m_gameView;
}

Controller* Game::addController(Controller* controller) {
    m_controllers.insert(controller);
    return controller;
}

void Game::deleteController(Controller* controller) {
    m_controllers.erase(controller);
    delete controller;
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

Player* Game::addPlayer(const std::string& name, Controller* controller,
                        Viper* viper) {
    Player* player = new Player(name, controller, viper);
    m_players.insert(player);
    PlayerPanel* panel = new PlayerPanel(m_statusBarView->getSize(), player);
    m_playerPanels.insert(panel);
    this->addObserver(panel, {GameEvent::EventType::Scoring});
    return player;
}

void Game::deletePlayer(Player* player) {
    for (auto panel : m_playerPanels) {
        if (panel->player() == player) {
            m_playerPanels.erase(panel);
            delete panel;
            break;  // Assumes only one panel per player
        }
    }
    m_players.erase(player);
    delete player;
}

Viper* Game::addViper(/* startConditions? */) {
    Viper* viper = new Viper();
    viper->setup(findFreeRect({100, 100}), Random::getDouble(0, 360), 5);
    viper->addObserver(this, {GameEvent::EventType::Destroy});
    m_collisionDetector.registerCollidable(viper);
    m_vipers.insert(viper);
    return viper;
}

void Game::killViper(Viper* viper) { viper->state(GameObject::Dying); }

void Game::deleteViper(Viper* viper) {
    m_collisionDetector.deRegisterCollidable(viper);
    m_vipers.erase(viper);
    delete viper;
}

void Game::addFood(Vec2 position, double diameter) {
    Food* food = new Food(position, diameter);
    // Check for collisions
    m_food.insert(food);
    m_collisionDetector.registerCollidable(food);
    food->addObserver(this, {GameEvent::EventType::Destroy});
}

void Game::deleteFood(Food* food) {
    m_collisionDetector.deRegisterCollidable(food);
    m_food.erase(food);
    delete food;
}

void Game::eatFood(Viper* viper, Food* food) {
    viper->addGrowth(1s * food->size() / Food::nominalFoodSize );
    food->state(GameObject::Dying);
    score_t score = 100 * food->size() / Food::nominalFoodSize;
    auto player = findPlayerWith(viper);
    player->score(score);
    ScoringEvent event(player, score);
    notify(&event);
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

Vec2 Game::findFreeRect(Vec2 rectSize, sf::Rect<double> limits) const {
    // If one million is not enough something is wrong or the level design is
    // bad
    const int maxTries = 1000000;
    ConvexBody* testRect =
        ConvexBody::createRectangle(Vec2(0, 0), rectSize, true);
    MonoBodyCollidable collidable(testRect);  // Takes ownership of testRect
    for (int i = 0; i < maxTries; ++i) {
        Vec2 position(
            Random::getDouble(limits.left, limits.width - rectSize.x),
            Random::getDouble(limits.top, limits.height - rectSize.y));
        testRect->convexShape.setPosition(position);
        testRect->updateBodyPart();

        if (m_collisionDetector.checkForCollisions(&collidable).empty())
            // Return center position
            return position + rectSize / 2;
    }
    throw std::runtime_error("Could not find an empty area.");
}

void Game::dispenseFood() {
    while (m_food.size() < 2 ) {
        double smallest = Food::nominalFoodSize / 2.;
        double largest = Food::nominalFoodSize * 1.5;
        double foodDiameter = Random::getDouble(smallest, largest);
        Food* food;
        // Find a spot, with some room to spare
        Vec2 centerPosition =
            findFreeRect(Vec2(2 * foodDiameter, 2 * foodDiameter));
        addFood(centerPosition, foodDiameter);
    }
}

void Game::draw() {
    clear(sf::Color::Black);
    setView(*m_statusBarView);
    for (const auto panel : m_playerPanels)
        RenderWindow::draw(*panel);
    setView(*m_gameView);
    RenderWindow::draw(*m_walls);
    for (const auto f : m_food)
        RenderWindow::draw(*f);
    for (const auto v : m_vipers)
        RenderWindow::draw(*v);
}

void Game::handleCollisions() {
    auto collisions = m_collisionDetector.checkForCollisions();
    for (auto& c : collisions)
        handleCollision(c);
}

void Game::handleCollision(const Colliders& colliders) {
    const Colliders reversed(colliders.second, colliders.first);
    for (auto& c : {colliders, reversed}) {
        const CollisionTriplet cA = c.first;
        const CollisionTriplet cB = c.second;
        if (typeid(*cA.collidable) == typeid(Viper)) {
            Viper* viper = (Viper*)cA.collidable;
            if (viper->state() == Viper::Alive &&
                viper->isSensitive(cA.bodypart)) {
                if (typeid(*cB.collidable) == typeid(Food)) {
                    Food* food = (Food*)cB.collidable;
                    if (food->state() == GameObject::Alive)
                        eatFood(viper, food);
                } else if (typeid(*cB.collidable) == typeid(Walls)) {
                    killViper(viper);
                } else if (typeid(*cB.collidable) == typeid(Viper)) {
                    killViper(viper);
                } else
                    throw std::runtime_error("Unknown collision happend");
            }
        }
    }
}

void Game::handleSteering() {
    for (auto player : m_players) {
        if (auto controller = player->controller()) {
            SteeringCommand cmd = controller->control();

            if (Viper* viper = player->viper()) {
                /* Boost if the steering event says so and either:
                 *    1) Boost in inactive and the boost power is full
                 *    2) Boost is active and the boost power is not depleted
                 */

                // This allows the viper a turning radius equal to its width
                const double maxAngularSpeed =
                    degPerRad * viper->turningRadius();
                double angularSpeed = cmd.turn * maxAngularSpeed;
                // Protect against erroneous input from controller
                if (std::abs(cmd.turn) > 1)
                    angularSpeed /= std::abs(cmd.turn);
                double boost = 0.;
                if (cmd.boost && ((viper->boost() > 0 and
                                   viper->boostCharge() > seconds(0)) or
                                  (viper->boost() == 0.0 and
                                   viper->boostCharge() == viper->boostMax())))
                    boost = 1.;
                viper->steer(angularSpeed, boost);
            }
        }
    }
}

void Game::handleDestruction(const DestroyEvent* event) {
    if (typeid(*event->objectPtr) == typeid(Viper))
        // we could retrieve the non-const ptr from the player but this is
        // easier
        deleteViper((Viper*)event->objectPtr);
    else if (typeid(*event->objectPtr) == typeid(Food))
        deleteFood((Food*)event->objectPtr);
    else
        throw std::runtime_error("Unknown object sending DestroyEvent.");
}

void Game::handleObjectUpdates(Time elapsedTime) {
    for (auto v : m_vipers)
        v->update(elapsedTime);
    for (auto f : m_food)
        f->update(elapsedTime);
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
    auto [beginDestroyEvents, endDestroyEvents] =
        m_eventsToBeProcessed.equal_range(GameEvent::EventType::Destroy);
    for (auto iter = beginDestroyEvents; iter != endDestroyEvents; ++iter)
        handleDestruction(static_cast<const DestroyEvent*>(iter->second));
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
    handleSteering();
    handleObjectUpdates(elapsedTime);
    handleCollisions();
    dispenseFood();
}

}  // namespace VVipers