#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <typeinfo>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/Scenes/ArenaScene.hpp>
#include <vvipers/Scenes/Collision/Bodypart.hpp>
#include <vvipers/Scenes/UIElements/Controller.hpp>
#include <vvipers/Scenes/GameElements/Player.hpp>
#include <vvipers/Scenes/GameElements/Viper.hpp>
#include <vvipers/Scenes/GameElements/Walls.hpp>
#include <vvipers/Scenes/GameOverScene.hpp>
#include <vvipers/Scenes/PauseScene.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;

ArenaScene::ArenaScene(const Game& game) : Scene(game) {
    Vec2 windowSize = getGame().getWindow().getSize();
    const sf::Vector2f statusBarRelSize(0.33, 0.1);
    const sf::Vector2f statusBarSize(windowSize.x * statusBarRelSize.x,
                                     windowSize.y * statusBarRelSize.y);
    m_statusBarView.setSize(statusBarSize);
    m_statusBarView.setCenter(statusBarSize / 2);
    m_statusBarView.setViewport(sf::FloatRect({0.f, 0.f}, statusBarRelSize));

    const sf::Vector2f gameRelSize(1, 1 - statusBarRelSize.y);
    const sf::Vector2f gameSize(windowSize.x * gameRelSize.x,
                                windowSize.y * gameRelSize.y);
    m_gameView.setSize(gameSize);
    m_gameView.setCenter(gameSize / 2);
    m_gameView.setViewport(
        sf::FloatRect({0.f, statusBarRelSize.y}, gameRelSize));

    // Only create one pause screen so that it can be reused
    m_pauseScene = std::make_shared<PauseScene>(getGame());

    // auto controllerM = addMouseController();
    // auto viperM = addViper();
    // auto playerM = addPlayer("PlayerM", controllerM, viperM);

    m_walls = std::make_unique<Walls>(gameSize);
    m_collisionDetector.registerCollidable(m_walls.get());

    auto controllerK = createKeyboardController();
    auto viperK = addViper();
    auto playerK = addPlayer("PlayerK", controllerK, viperK);
    playerK->color(sf::Color::Red);
}

ArenaScene::~ArenaScene() {}

controller_ptr ArenaScene::addController(controller_ptr controller) {
    m_controllers.insert(controller);
    return controller;
}

void ArenaScene::deleteController(controller_ptr controller) {
    m_controllers.erase(controller);
}

controller_ptr ArenaScene::createMouseController() {
    //getGame().getWindow().setMouseCursorGrabbed(true);
    //getGame().getWindow().setMouseCursorVisible(false);
    Vec2 windowSize = getGame().getWindow().getSize();
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2),
                           getGame().getWindow());
    return addController(std::make_shared<MouseController>(getGame().getWindow()));
}

controller_ptr ArenaScene::createKeyboardController() {
    KeyboardController::KeyboardControls keys;
    keys.left = sf::Keyboard::Left;
    keys.right = sf::Keyboard::Right;
    keys.boost = sf::Keyboard::Up;
    return addController(std::make_shared<KeyboardController>(keys));
}

player_ptr ArenaScene::addPlayer(const std::string& name, controller_ptr controller,
                            viper_ptr viper) {
    auto player = make_shared<Player>(name, controller, viper);
    m_players.insert(player);
    auto panel = std::make_unique<PlayerPanel>(
        m_statusBarView.getSize(), player.get(), getGame().getFontService());
    viper->addObserver(panel.get(), {GameEvent::EventType::ObjectModified});
    player->addObserver(panel.get(), {GameEvent::EventType::ObjectModified});
    // Container takes ownership, ptr panel is no longer valid
    m_playerPanels.insert(std::move(panel));
    return player;
}

void ArenaScene::deletePlayer(player_ptr player) {
    for (auto& panel : m_playerPanels) {
        if (panel->getPlayer() == player.get()) {
            m_playerPanels.erase(panel);
            break;  // Assumes only one panel per player
        }
    }
    m_players.erase(player);
}

viper_ptr ArenaScene::addViper(/* startConditions? */) {
    auto viper = std::make_shared<Viper>(getGame().getOptionsService(),
                                         getGame().getTextureService());
    viper->setup(findFreeRect({100, 100}), Random::getDouble(0, 360), 5);
    viper->addObserver(this, {GameEvent::EventType::Destroy});
    m_collisionDetector.registerCollidable(viper.get());
    // Container takes ownership, ptr viper is no longer valid
    m_vipers.insert(viper);
    return viper;
}

void ArenaScene::killViper(Viper* viper) { viper->state(GameObject::Dying); }

void ArenaScene::deleteViper(Viper* viper) {
    m_collisionDetector.deRegisterCollidable(viper);
    for (auto& v : m_vipers)
        if (v.get() == viper) {
            m_vipers.erase(v);
            break;
        }
}

void ArenaScene::addFood(Vec2 position, double diameter) {
    auto food = std::make_unique<Food>(position, diameter);
    // Check for collisions
    m_collisionDetector.registerCollidable(food.get());
    food->addObserver(this, {GameEvent::EventType::Destroy});
    // Container takes ownership, ptr food is no longer valid
    m_food.insert(std::move(food));
}

void ArenaScene::deleteFood(Food* food) {
    m_collisionDetector.deRegisterCollidable(food);
    for (auto& f : m_food)
        if (f.get() == food) {
            m_food.erase(f);
            break;
        }
}

void ArenaScene::eatFood(Viper* viper, Food* food) {
    viper->addGrowth(1s * food->getSize() / Food::nominalFoodSize);
    food->state(GameObject::Dying);
    score_t score = 100 * food->getSize() / Food::nominalFoodSize;
    auto player = findPlayerWith(viper);
    player->score(score);

    PlayerPanel* panel = findPlayerPanel(player);
    auto flyingScore = std::make_unique<FlyingScore>(
        Vec2(getGame().getWindow().mapCoordsToPixel(food->getPosition(),
                                                 m_gameView)),
        4 * viper->velocity(),
        Vec2(getGame().getWindow().mapCoordsToPixel(panel->getScoreTarget(),
                                                 m_statusBarView)),
        1s, score, getGame().getFontService());
    flyingScore->setColor(sf::Color::Magenta, sf::Color::Red);
    flyingScore->setFontSize(0.03 * getGame().getWindow().getSize().y, 1.0);
    flyingScore->addObserver(this, {GameEvent::EventType::Destroy});
    flyingScore->addObserver(panel, {GameEvent::EventType::Scoring});
    m_flyingScores.insert(std::move(flyingScore));
}

PlayerPanel* ArenaScene::findPlayerPanel(const Player* player) const {
    for (auto& panel : m_playerPanels)
        if (player == panel->getPlayer())
            return panel.get();
    return nullptr;
}

Player* ArenaScene::findPlayerWith(const Controller* controller) const {
    for (auto player : m_players)
        if (player->controller() == controller)
            return player.get();
    return nullptr;
}

Player* ArenaScene::findPlayerWith(const Viper* viper) const {
    for (auto player : m_players)
        if (player->viper() == viper)
            return player.get();
    return nullptr;
}

Vec2 ArenaScene::findFreeRect(Vec2 rectSize, sf::Rect<double> limits) const {
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

void ArenaScene::dispenseFood() {
    while (m_food.size() < 2) {
        double smallest = Food::nominalFoodSize / 2.;
        double largest = Food::nominalFoodSize * 1.5;
        double foodDiameter = Random::getDouble(smallest, largest);
        // Find a spot, with some room to spare
        Vec2 centerPosition =
            findFreeRect(Vec2(2 * foodDiameter, 2 * foodDiameter));
        addFood(centerPosition, foodDiameter);
    }
}

void ArenaScene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.clear(sf::Color::Black);
    target.setView(m_statusBarView);
    for (const auto& panel : m_playerPanels)
        target.draw(*panel,states);
    target.setView(m_gameView);
    target.draw(*m_walls,states);
    for (const auto& f : m_food)
        target.draw(*f,states);
    for (const auto& v : m_vipers)
        target.draw(*v,states);
    target.setView(target.getDefaultView());
    for (const auto& s : m_flyingScores)
        target.draw(*s,states);
}

void ArenaScene::handleCollisions() {
    auto collisions = m_collisionDetector.checkForCollisions();
    for (auto& c : collisions)
        handleCollision(c);
}

void ArenaScene::handleCollision(const Colliders& colliders) {
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

void ArenaScene::handleSteering() {
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

void ArenaScene::handleDestruction(const DestroyEvent* event) {
    if (typeid(*event->objectPtr) == typeid(Viper))
        // we could retrieve the non-const ptr from the player but this is
        // easier
        deleteViper((Viper*)event->objectPtr);
    else if (typeid(*event->objectPtr) == typeid(Food))
        deleteFood((Food*)event->objectPtr);
    else if (typeid(*event->objectPtr) == typeid(FlyingScore)) {
        for (auto& fs : m_flyingScores) {
            if (fs.get() == event->objectPtr) {
                m_flyingScores.erase(fs);
                break;
            }
        }
    } else
        throw std::runtime_error("Unknown object sending DestroyEvent.");
}

void ArenaScene::handleObjectUpdates(Time elapsedTime) {
    for (auto& v : m_vipers)
        v->update(elapsedTime);
    for (auto& f : m_food)
        f->update(elapsedTime);
    for (auto& s : m_flyingScores)
        s->update(elapsedTime);
}

void ArenaScene::checkForGameOver() {
    for (auto player : m_players)
        if (player->viper() && player->viper()->state() == GameObject::Alive)
            return;  // Keep on playing as long as someone is alive

    setTransitionState(TransitionState::Spawn);
    // This scene must be kept alive since GameOverScreen reads
    std::vector<std::shared_ptr<const Player>> players;
    for (auto p : m_players)
        players.push_back(p);
    m_transitionScene = make_shared<GameOverScene>(getGame(), players);
}

void ArenaScene::onNotify(const GameEvent* event) {
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

void ArenaScene::processEvent(const sf::Event& event) {
        switch (event.type) {
            case sf::Event::Closed: {
                setSceneState(SceneState::Paused);
                setTransitionState(TransitionState::Return);
                break;
            }
            case sf::Event::Resized: {
                break;
            }
            case sf::Event::KeyPressed: {
                switch (event.key.code) {
                    case sf::Keyboard::Escape: {
                        setSceneState(SceneState::Paused);
                        setTransitionState(TransitionState::Spawn);
                        m_transitionScene = m_pauseScene;
                        break;
                    }
                    default: {
                        break;
                    }
                }
                break;
            }
            default: {
                break;
            }
        }
}

void ArenaScene::processGameEvents() {
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

void ArenaScene::update(Time elapsedTime) {
    handleSteering();
    handleObjectUpdates(elapsedTime);
    handleCollisions();
    dispenseFood();
    processGameEvents();
    checkForGameOver();
}

std::shared_ptr<Scene> ArenaScene::makeTransition() {
    // Setting the state after the transition has occured and this scene starts
    // running again
    setTransitionState(TransitionState::Continue);
    return m_transitionScene;
}
}  // namespace VVipers