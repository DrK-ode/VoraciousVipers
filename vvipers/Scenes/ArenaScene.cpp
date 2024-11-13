#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath>
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

#include "vvipers/Collisions/CollidingSegment.hpp"
#include "vvipers/Utilities/Time.hpp"
#include "vvipers/Utilities/VVMath.hpp"

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
    _gameView.setSize(gameSize);
    _gameView.setCenter(gameSize / 2);
    _gameView.setViewport(
        sf::FloatRect({0.f, statusBarRelSize.y}, gameRelSize));

    _walls = std::make_unique<Walls>(gameSize);
    _collision_manager.register_colliding_body(*_walls.get());

    // The players must absolutely be added _after_ the level has been filled
    // with obstacles, otherwise they might end up inside or on top of them.
    dispenseFood();
    addPlayers(playerNames, playerPrimaryColors, playerSecondaryColors,
               playerKeys, statusBarViews);

    // Only create one pause screen so that it can be reused
    _pauseScene = std::make_shared<PauseScene>(getGame());
}

ArenaScene::~ArenaScene() {}

void ArenaScene::addPlayers(std::vector<std::string>& playerNames,
                            std::vector<std::string>& primaryColors,
                            std::vector<std::string>& secondaryColors,
                            std::vector<double>& playerKeys,
                            std::vector<sf::View>& playerViews) {
    auto numberOfPlayers = playerNames.size();
    // Vipers are added to the member vector _vipers
    addVipers(numberOfPlayers);
    for (size_t i = 0; i < numberOfPlayers; ++i) {
        KeyboardController::KeyboardControls keys;
        keys.left = sf::Keyboard::Key(playerKeys[3 * i + 0]);
        keys.right = sf::Keyboard::Key(playerKeys[3 * i + 1]);
        keys.boost = sf::Keyboard::Key(playerKeys[3 * i + 2]);
        auto controller = createController(keys);
        auto player =
            addPlayer(playerNames[i], colorFromRGBString(primaryColors[i]),
                      colorFromRGBString(secondaryColors[i]), controller,
                      _vipers[i], playerViews[i]);
    }
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
    _players.push_back(player);
    auto panel = std::make_unique<PlayerPanel>(view, player.get(),
                                               getGame().getFontService());
    viper->addObserver(panel.get(), {GameEvent::EventType::ObjectModified});
    player->addObserver(panel.get(), {GameEvent::EventType::ObjectModified});
    // Container takes ownership, ptr panel is no longer valid
    _player_panels.push_back(std::move(panel));
    return player;
}

void ArenaScene::deletePlayer(player_ptr player) {
    for (auto panel_iter = _player_panels.begin();
         panel_iter != _player_panels.end(); ++panel_iter) {
        if ((*panel_iter)->getPlayer() == player.get()) {
            _player_panels.erase(panel_iter);
            break;  // Assumes only one panel per player
        }
    }
    _players.erase(std::find(_players.begin(), _players.end(), player));
}

void ArenaScene::addVipers(size_t number_of_vipers) {
    for (size_t i = 0; i < number_of_vipers; ++i) {
        auto viper = std::make_shared<Viper>(getGame().getOptionsService(),
                                             getGame().getTextureService());

        double length = viper->speed() * 5;  // 5s free space
        double width = viper->nominal_width();
        bool allowRotation = true;
        // Give some margin to the width
        auto [center, angle] =
            find_free_rectangular_space(length, 1.5 * width, allowRotation);

        Vec2 direction = Vec2(1, 0).rotate(angle);
        Vec2 viper_tail_position = center - 0.5 * length * direction;
        viper->setup(viper_tail_position, angle, 50);

        viper->addObserver(this, {GameEvent::EventType::Destroy});
        _collision_manager.register_colliding_body(*viper.get());
        // Container takes ownership, ptr viper is no longer valid
        _vipers.push_back(viper);
    }
}

void ArenaScene::killViper(Viper* viper) {
    // You cannot kill what's already dead
    if (viper->state() == GameObject::Alive)
        viper->state(GameObject::Dying);
}

void ArenaScene::deleteViper(Viper* viper) {
    _collision_manager.deregister_colliding_body(*viper);
    for (auto viper_iter = _vipers.begin(); viper_iter != _vipers.end();
         ++viper_iter)
        if ((*viper_iter).get() == viper) {
            _vipers.erase(viper_iter);
            break;
        }
}

void ArenaScene::addFood(Vec2 position, double diameter) {
    auto food = std::make_unique<Food>(
        position, diameter, timeFromSeconds(Random::getDouble(5, 10)),
        getGame().getColorService().getColor(Random::getInt()));
    // Check for collisions
    _collision_manager.register_colliding_body(*food.get());
    food->addObserver(this, {GameEvent::EventType::Destroy});
    // Container takes ownership, ptr food is no longer valid
    _food.push_back(std::move(food));
}

void ArenaScene::deleteFood(Food* food) {
    _collision_manager.deregister_colliding_body(*food);
    for (auto food_iter = _food.begin(); food_iter != _food.end(); ++food_iter)
        if ((*food_iter).get() == food) {
            _food.erase(food_iter);
            break;
        }
}

void ArenaScene::eatFood(Viper* viper, Food* food) {
    viper->eat(*food);
    food->state(GameObject::Dying);
    score_t score = food->score_value();
    auto player = findPlayerWith(viper);
    player->score(score);

    PlayerPanel* panel = findPlayerPanel(player);
    auto flyingScore = std::make_unique<FlyingScore>(
        Vec2(getGame().getWindow().mapCoordsToPixel(food->getPosition(),
                                                    _gameView)),
        4 * viper->velocity(),
        Vec2(getGame().getWindow().mapCoordsToPixel(panel->getScoreTarget(),
                                                    panel->getView())),
        1s, score, getGame().getFontService());
    flyingScore->setColor(sf::Color::Magenta, sf::Color::Red);
    flyingScore->setFontSize(0.03 * getGame().getWindow().getSize().y, 1.0);
    flyingScore->addObserver(this, {GameEvent::EventType::Destroy});
    flyingScore->addObserver(panel, {GameEvent::EventType::Scoring});
    _flying_scores.push_back(std::move(flyingScore));
}

PlayerPanel* ArenaScene::findPlayerPanel(const Player* player) const {
    for (auto& panel : _player_panels)
        if (player == panel->getPlayer())
            return panel.get();
    return nullptr;
}

Player* ArenaScene::findPlayerWith(const Controller* controller) const {
    for (auto player : _players)
        if (player->controller() == controller)
            return player.get();
    return nullptr;
}

Player* ArenaScene::findPlayerWith(const Viper* viper) const {
    for (auto player : _players)
        if (player->viper() == viper)
            return player.get();
    return nullptr;
}

Vec2 ArenaScene::find_free_circular_space(double radius) {
    auto game_area =
        sf::Rect<double>(0, 0, _gameView.getSize().x, _gameView.getSize().y);
    const int max_attempts = 1000000;
    for (int i = 0; i < max_attempts; ++i) {
        Vec2 center(Random::getDouble(game_area.left, game_area.width),
                    Random::getDouble(game_area.top, game_area.height));
        if (!_collision_manager.is_circle_occupied(center, radius))
            return center;
    }
    throw std::runtime_error("Could not find an empty area.");
}

std::pair<Vec2, double> ArenaScene::find_free_rectangular_space(
    double width, double height, bool allow_rotation) {
    auto game_area_size = Vec2(_gameView.getSize().x, _gameView.getSize().y);
    const int max_attempts = 1000000;
    for (int i = 0; i < max_attempts; ++i) {
        Vec2 center(Random::getDouble(0., game_area_size.x),
                    Random::getDouble(0., game_area_size.y));
        double angle = allow_rotation ? Random::getDouble(0, twopi) : 0.;
        if (!_collision_manager.is_rectangle_occupied(center, width, height,
                                                      angle))
            return {center, angle};
    }
    throw std::runtime_error("Could not find an empty area.");
}

void ArenaScene::dispenseFood() {
    while (_food.size() < 2) {
        double smallest = Food::nominalFoodRadius * 0.75;
        double largest = Food::nominalFoodRadius * 1.25;
        double food_radius = std::sqrt(
            Random::getDouble(smallest * smallest, largest * largest));
        // Find a spot, with some room to spare
        addFood(find_free_circular_space(2 * food_radius), food_radius);
    }
}

void ArenaScene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.clear(sf::Color::Black);
    for (const auto& panel : _player_panels) {
        target.setView(panel->getView());
        target.draw(*panel, states);
    }
    target.setView(_gameView);
    target.draw(*_walls, states);
    for (const auto& f : _food)
        target.draw(*f, states);
    for (const auto& v : _vipers)
        target.draw(*v, states);
    target.setView(target.getDefaultView());
    for (const auto& s : _flying_scores)
        target.draw(*s, states);
}

void ArenaScene::handleCollisions() {
    for (auto& collision : _collision_manager.check_for_collisions()) {
        handleCollision(collision);
    }
}

void ArenaScene::handleCollision(const CollisionPair& collision) {
    /* Only collisions involving a viper is interesting so both ColliderSegments
     * need to be checked in case they are a viper. If both are vipers we check
     * for consequences one viper at a time. */
    for (const auto& collider_segment : {collision.first, collision.second}) {
        // A collidee is what the collider collides into :)
        const auto collidee_segment =
            collider_segment->base_object() == collision.first->base_object()
                ? collision.second
                : collision.first;
        // If it's not a viper, we don't care!
        const CollidingBody* collider_base_object =
            collider_segment->base_object();
        if (typeid(*collider_base_object) == typeid(Viper)) {
            Viper* collider_viper = (Viper*)&collider_base_object;
            if (collider_viper->state() == Viper::Alive &&
                collider_segment->segment_id() == 0) {
                const CollidingBody* collidee_base_object =
                    collidee_segment->base_object();
                if (typeid(*collidee_base_object) == typeid(Food)) {
                    Food* food = (Food*)&collidee_base_object;
                    if (food->state() == GameObject::Alive)
                        eatFood(collider_viper, food);
                } else if (typeid(*collidee_base_object) == typeid(Walls)) {
                    tagDebug("Killed by collision between Viper segment ",
                             collider_segment->segment_id(),
                             " and wall segment ",
                             collidee_segment->segment_id());
                    killViper(collider_viper);
                } else if (typeid(*collidee_base_object) == typeid(Viper)) {
                    // Self collision
                    if (collider_viper == (Viper*)&collidee_base_object) {
                        auto segment_distance =
                            std::abs(int(collider_segment->segment_id()) -
                                     int(collidee_segment->segment_id()));
                        // Check if it's just neighbouring segments touching or
                        // a real collision
                        if (segment_distance > 1) {
                            tagDebug(
                                "Killed by collision between Viper segment ",
                                collider_segment->segment_id(),
                                " and Viper segment ",
                                collidee_segment->segment_id());
                            killViper(collider_viper);
                        }
                    }
                    // Collision with other viper
                    else
                        killViper(collider_viper);
                } else
                    throw std::runtime_error("Unknown collision happend");
            }
        }
    }
}

void ArenaScene::handleSteering() {
    for (auto player : _players) {
        if (auto controller = player->controller()) {
            SteeringCommand cmd = controller->control();

            if (Viper* viper = player->viper()) {
                // Protect against erroneous input from controller
                if (std::abs(cmd.turn) > 1)
                    cmd.turn /= std::abs(cmd.turn);
                double angularSpeed =
                    cmd.turn * degPerRad * viper->max_angular_speed();
                /* Boost if the steering event says so and either:
                 *    1) Boost in inactive and the boost power is full
                 *    2) Boost is active and the boost power is not depleted
                 */
                double boost = 0.;
                if (cmd.boost &&
                    ((viper->boost_amount() > 0 and
                      viper->boost_charge() > timeFromSeconds(0)) or
                     (viper->boost_amount() == 0.0 and
                      viper->boost_charge() == viper->boost_max())))
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
        for (auto flying_score_iter = _flying_scores.begin();
             flying_score_iter != _flying_scores.end(); ++flying_score_iter) {
            if ((*flying_score_iter).get() == event->objectPtr) {
                _flying_scores.erase(flying_score_iter);
                break;
            }
        }
    } else
        throw std::runtime_error("Unknown object sending DestroyEvent.");
}

void ArenaScene::handleObjectUpdates(Time elapsedTime) {
    for (auto& v : _vipers)
        v->update(elapsedTime);
    for (auto& f : _food)
        f->update(elapsedTime);
    for (auto& s : _flying_scores)
        s->update(elapsedTime);
}

void ArenaScene::checkForGameOver() {
    for (auto player : _players)
        if (player->viper() && player->viper()->state() == GameObject::Alive)
            return;  // Keep on playing as long as someone is alive

    setTransitionState(TransitionState::Spawn);
    // This scene must be kept alive since GameOverScreen reads
    std::vector<std::shared_ptr<const Player>> players;
    for (auto p : _players)
        players.push_back(p);
    _transitionScene = make_shared<GameOverScene>(getGame(), players);
}

void ArenaScene::onNotify(const GameEvent* event) {
    // If directly processable, do it!
    switch (event->type()) {
        default: {
            // Otherwise save it for later
            _events_to_be_processed.insert(
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
                    _transitionScene = _pauseScene;
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
        _events_to_be_processed.equal_range(GameEvent::EventType::Destroy);
    for (auto iter = beginDestroyEvents; iter != endDestroyEvents; ++iter)
        handleDestruction(static_cast<const DestroyEvent*>(iter->second));
    // Delete fully processed events?
    // For now just delete all...
    for (auto& event : _events_to_be_processed)
        delete event.second;
    _events_to_be_processed.clear();
}

void ArenaScene::update(Time elapsedTime) {
    handleSteering();
    Stopwatch clock;
    clock.start();
    handleObjectUpdates(elapsedTime);
    tagInfo("Game object updates took: ", clock.split());
    handleCollisions();
    tagInfo("Collision handling took: ", clock.split());
    dispenseFood();
    processGameEvents();
    checkForGameOver();
}

std::shared_ptr<Scene> ArenaScene::makeTransition() { return _transitionScene; }
}  // namespace VVipers