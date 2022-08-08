#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <typeinfo>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/GameElements/Player.hpp>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/GameElements/Walls.hpp>
#include <vvipers/Scenes/ArenaScene.hpp>
#include <vvipers/Scenes/GameOverScene.hpp>
#include <vvipers/Scenes/PauseScene.hpp>
#include <vvipers/UIElements/Controller.hpp>
#include <vvipers/Utilities/VVColor.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;

ArenaScene::ArenaScene(Game& game) : Scene(game) {
    size_t numberOfPlayers =
        game.getOptionsService().getOptionDouble("Players/numberOfPlayers");
    auto playerNames =
        game.getOptionsService().getOptionStringArray("Players/names");
    auto playerPrimaryColors =
        game.getOptionsService().getOptionStringArray("Players/primaryColors");
    auto playerSecondaryColors = game.getOptionsService().getOptionStringArray(
        "Players/secondaryColors");
    auto playerKeys =
        game.getOptionsService().getOptionDoubleArray("Players/keys");
    auto numberOfDivisions = numberOfPlayers + 1;
    if ((playerNames.size() < numberOfPlayers) or
        (playerPrimaryColors.size() < numberOfPlayers) or
        (playerSecondaryColors.size() < numberOfPlayers) or
        (playerKeys.size() < 3 * numberOfPlayers)) {
        throw std::runtime_error(
            "Wrong size of arrays in player configuration.");
    }
    playerNames.resize(numberOfPlayers);
    playerPrimaryColors.resize(numberOfPlayers);
    playerSecondaryColors.resize(numberOfPlayers);
    playerKeys.resize(3 * numberOfPlayers);

    Vec2 windowSize = getGame().getWindow().getSize();
    const sf::Vector2f statusBarRelSize(1. / numberOfDivisions, 0.1);
    const sf::Vector2f statusBarSize(windowSize.x * statusBarRelSize.x,
                                     windowSize.y * statusBarRelSize.y);

    std::vector<sf::View> statusBarViews;
    statusBarViews.resize(numberOfDivisions);
    for (size_t i = 0; i < numberOfDivisions; ++i) {
        statusBarViews[i].setSize(statusBarSize);
        statusBarViews[i].setCenter(0.5 * statusBarSize.x,
                                    0.5 * statusBarSize.y);
        statusBarViews[i].setViewport(
            sf::FloatRect({i * statusBarRelSize.x, 0.}, statusBarRelSize));
    }

    const sf::Vector2f gameRelSize(1, 1 - statusBarRelSize.y);
    const sf::Vector2f gameSize(windowSize.x * gameRelSize.x,
                                windowSize.y * gameRelSize.y);
    m_gameView.setSize(gameSize);
    m_gameView.setCenter(gameSize / 2);
    m_gameView.setViewport(
        sf::FloatRect({0.f, statusBarRelSize.y}, gameRelSize));

    m_walls = std::make_unique<Walls>(gameSize);
    m_colliderManager.registerCollider(*m_walls.get());

    // The players must absolutely be added _after_ the level has been filled
    // with obstacles, otherwise they might end up inside or on top of them.
    dispenseFood();
    addPlayers(playerNames, playerPrimaryColors, playerSecondaryColors,
               playerKeys, statusBarViews);

    // Only create one pause screen so that it can be reused
    m_pauseScene = std::make_shared<PauseScene>(getGame());
}

ArenaScene::~ArenaScene() {}

void ArenaScene::addPlayers(std::vector<std::string>& playerNames,
                            std::vector<std::string>& primaryColors,
                            std::vector<std::string>& secondaryColors,
                            std::vector<double>& playerKeys,
                            std::vector<sf::View>& playerViews) {
    auto numberOfPlayers = playerNames.size();
    std::vector<const Collider*> otherViperStartAreas;
    for (size_t i = 0; i < numberOfPlayers; ++i) {
        KeyboardController::KeyboardControls keys;
        keys.left = sf::Keyboard::Key(playerKeys[3 * i + 0]);
        keys.right = sf::Keyboard::Key(playerKeys[3 * i + 1]);
        keys.boost = sf::Keyboard::Key(playerKeys[3 * i + 2]);
        auto controller = createController(keys);
        auto viper = addViper(otherViperStartAreas);
        auto player =
            addPlayer(playerNames[i], colorFromRGBString(primaryColors[i]),
                      colorFromRGBString(secondaryColors[i]), controller, viper,
                      playerViews[i]);
    }
    for (auto area : otherViperStartAreas)
        delete area;
}

controller_ptr ArenaScene::createController(
    const KeyboardController::KeyboardControls& keys) {
    if ((keys.left == sf::Keyboard::Unknown) and
        (keys.right == sf::Keyboard::Unknown) and
        (keys.boost == sf::Keyboard::Unknown)) {
        return std::make_shared<MouseController>(getGame());
    } else if ((keys.left != sf::Keyboard::Unknown) and
               (keys.right != sf::Keyboard::Unknown) and
               (keys.boost != sf::Keyboard::Unknown)) {
        return std::make_shared<KeyboardController>(keys);
    } else
        throw std::runtime_error("Inconsistently set controller keys.");
}

player_ptr ArenaScene::addPlayer(const std::string& name,
                                 sf::Color primaryColor,
                                 sf::Color secondaryColor,
                                 controller_ptr controller, viper_ptr viper,
                                 sf::View view) {
    auto player = make_shared<Player>(name, controller, viper);
    player->setColors(primaryColor, secondaryColor);
    m_players.insert(player);
    auto panel = std::make_unique<PlayerPanel>(view, player.get(),
                                               getGame().getFontService());
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

viper_ptr ArenaScene::addViper(
    std::vector<const Collider*>& otherViperStartAreas) {
    auto viper = std::make_shared<Viper>(getGame().getOptionsService(),
                                         getGame().getTextureService());

    double length = viper->getSpeed() * 5;  // 5s free space
    double width = viper->getNominalWidth();
    bool allowRotation = true;
    // Give some margin to the width
    RectangleShape* viperZone = new RectangleShape({length, 1.5 * width});
    findFreeSpace(
        *viperZone, allowRotation, otherViperStartAreas,
        sf::Rect<double>(0, 0, m_gameView.getSize().x, m_gameView.getSize().y));
    otherViperStartAreas.push_back(viperZone);

    double angle = viperZone->getRotation();
    Vec2 direction = Vec2(1, 0).rotate(angle);
    Vec2 offset = 0.5 * width * direction;
    Vec2 viperPosition = viperZone->getPosition() + offset + offset.perpVec();
    viper->setup(viperPosition, angle, 0);

    viper->addObserver(this, {GameEvent::EventType::Destroy});
    m_colliderManager.registerCollider(*viper.get());
    // Container takes ownership, ptr viper is no longer valid
    m_vipers.insert(viper);
    return viper;
}

void ArenaScene::killViper(Viper* viper) {
    // You cannot kill what's already dead
    if (viper->state() == GameObject::Alive)
        viper->state(GameObject::Dying);
}

void ArenaScene::deleteViper(Viper* viper) {
    m_colliderManager.deRegisterCollider(*viper);
    for (auto& v : m_vipers)
        if (v.get() == viper) {
            m_vipers.erase(v);
            break;
        }
}

void ArenaScene::addFood(Vec2 position, double diameter) {
    auto food = std::make_unique<Food>(
        position, diameter, timeFromseconds(Random::getDouble(5, 10)),
        getGame().getColorService().getColor(Random::getInt()));
    // Check for collisions
    m_colliderManager.registerCollider(*food.get());
    food->addObserver(this, {GameEvent::EventType::Destroy});
    // Container takes ownership, ptr food is no longer valid
    m_food.insert(std::move(food));
}

void ArenaScene::deleteFood(Food* food) {
    m_colliderManager.deRegisterCollider(*food);
    for (auto& f : m_food)
        if (f.get() == food) {
            m_food.erase(f);
            break;
        }
}

void ArenaScene::eatFood(Viper* viper, Food* food) {
    viper->eat(*food);
    food->state(GameObject::Dying);
    score_t score = food->getScoreValue();
    auto player = findPlayerWith(viper);
    player->score(score);

    PlayerPanel* panel = findPlayerPanel(player);
    auto flyingScore = std::make_unique<FlyingScore>(
        Vec2(getGame().getWindow().mapCoordsToPixel(food->getPosition(),
                                                    m_gameView)),
        4 * viper->getVelocity(),
        Vec2(getGame().getWindow().mapCoordsToPixel(panel->getScoreTarget(),
                                                    panel->getView())),
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

template <typename T>
Vec2 ArenaScene::findFreeSpace(T& testObject, bool allowRotation) const {
    std::vector<const Collider*> dummy;
    auto gameArea =
        sf::Rect<double>(0, 0, m_gameView.getSize().x, m_gameView.getSize().y);
    return findFreeSpace(testObject, allowRotation, dummy, gameArea);
}

template <typename T>
Vec2 ArenaScene::findFreeSpace(
    T& testObject, bool allowRotation,
    const std::vector<const Collider*>& excludedRegions,
    sf::Rect<double> limits) const {
    // If one million is not enough something is wrong or the level design is
    // bad
    const int maxTries = 1000000;
    for (int i = 0; i < maxTries; ++i) {
        Vec2 position(Random::getDouble(limits.left, limits.width),
                      Random::getDouble(limits.top, limits.height));
        testObject.setPosition(position);
        if (allowRotation) {
            testObject.setRotation(Random::getDouble(0, 360));
        }
        // First, check against all excluded regions
        bool occupied = false;
        for (auto excluded : excludedRegions) {
            if (Collider::collision(testObject, *excluded).size() > 0) {
                occupied = true;
                break;
            }
        }
        if (occupied)
            continue;
        // Second, check against all existing objects in the game

        if (m_colliderManager.checkForCollisions(testObject).empty())
            // Return center position
            return position;
    }
    throw std::runtime_error("Could not find an empty area.");
}

void ArenaScene::dispenseFood() {
    while (m_food.size() < 2) {
        double smallest = Food::nominalFoodRadius / 2.;
        double largest = Food::nominalFoodRadius * 1.5;
        double foodRadius = std::sqrt(
            Random::getDouble(smallest * smallest, largest * largest));
        // Find a spot, with some room to spare
        CircleShape testCircle(foodRadius * 2);
        findFreeSpace(testCircle);
        addFood(testCircle.getPosition(), foodRadius);
    }
}

void ArenaScene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.clear(sf::Color::Black);
    for (const auto& panel : m_playerPanels) {
        target.setView(panel->getView());
        target.draw(*panel, states);
    }
    target.setView(m_gameView);
    target.draw(*m_walls, states);
    for (const auto& f : m_food)
        target.draw(*f, states);
    for (const auto& v : m_vipers)
        target.draw(*v, states);
    target.setView(target.getDefaultView());
    for (const auto& s : m_flyingScores)
        target.draw(*s, states);
}

void ArenaScene::handleCollisions() {
    for (auto& c : m_colliderManager.checkForCollisions()) {
        handleCollision(c.colliderA, c.colliderB);
        handleCollision(c.colliderB, c.colliderA);
    }
}

void ArenaScene::handleCollision(const ColliderSegment& cA,
                                 const ColliderSegment& cB) {
    if (typeid(*cA.collider) == typeid(Viper)) {
        Viper* viper = (Viper*)cA.collider;
        if (viper->state() == Viper::Alive &&
            viper->isSegmentActive(cA.segmentIndex)) {
            if (typeid(*cB.collider) == typeid(Food)) {
                Food* food = (Food*)cB.collider;
                if (food->state() == GameObject::Alive)
                    eatFood(viper, food);
            } else if (typeid(*cB.collider) == typeid(Walls)) {
                tagDebug("Killed by collision between Viper segment ",
                         cA.segmentIndex, " and wall segment ",
                         cB.segmentIndex);
                killViper(viper);
            } else if (typeid(*cB.collider) == typeid(Viper)) {
                auto dindex = cA.segmentIndex > cB.segmentIndex
                                  ? cA.segmentIndex - cB.segmentIndex
                                  : cB.segmentIndex - cA.segmentIndex;
                // Check if it's just neighbouring segments touching or a real
                // collision
                if (dindex > 1) {
                    tagDebug("Killed by collision between Viper segments ",
                             cA.segmentIndex, " and ", cB.segmentIndex);
                    killViper(viper);
                }
            } else
                throw std::runtime_error("Unknown collision happend");
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

                // Protect against erroneous input from controller
                if (std::abs(cmd.turn) > 1)
                    cmd.turn /= std::abs(cmd.turn);
                double angularSpeed =
                    cmd.turn * degPerRad * viper->getMaxAngularSpeed();
                double boost = 0.;
                if (cmd.boost &&
                    ((viper->getBoost() > 0 and
                      viper->getBoostCharge() > timeFromseconds(0)) or
                     (viper->getBoost() == 0.0 and
                      viper->getBoostCharge() == viper->getBoostMax())))
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
            setTransitionState(TransitionState::Quit);
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