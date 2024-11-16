#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
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

#include "vvipers/UIElements/PlayerPanel.hpp"
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
    _game_view.setSize(gameSize);
    _game_view.setCenter(gameSize / 2);
    _game_view.setViewport(
        sf::FloatRect({0.f, statusBarRelSize.y}, gameRelSize));

    _walls = std::make_unique<Walls>(gameSize);
    _collision_manager.register_colliding_body(_walls.get());

    // The players must absolutely be added _after_ the level has been filled
    // with obstacles, otherwise they might end up inside or on top of them.
    dispense_food();
    add_players(playerNames, playerPrimaryColors, playerSecondaryColors,
               playerKeys, statusBarViews);

    // Only create one pause screen so that it can be reused
    _pause_scene = std::make_shared<PauseScene>(getGame());
}

ArenaScene::~ArenaScene() {}

void ArenaScene::add_players(std::vector<std::string>& playerNames,
                            std::vector<std::string>& primaryColors,
                            std::vector<std::string>& secondaryColors,
                            std::vector<double>& playerKeys,
                            std::vector<sf::View>& playerViews) {
    auto numberOfPlayers = playerNames.size();
    // Vipers are added to the member vector _vipers
    add_vipers(numberOfPlayers);
    for (size_t i = 0; i < numberOfPlayers; ++i) {
        KeyboardController::KeyboardControls keys;
        keys.left = sf::Keyboard::Key(playerKeys[3 * i + 0]);
        keys.right = sf::Keyboard::Key(playerKeys[3 * i + 1]);
        keys.boost = sf::Keyboard::Key(playerKeys[3 * i + 2]);
        auto controller = create_controller(keys);
        add_player(playerNames[i], colorFromRGBString(primaryColors[i]),
                  colorFromRGBString(secondaryColors[i]), std::move(controller),
                  _vipers[i].get(), playerViews[i]);
    }
}

std::unique_ptr<Controller> ArenaScene::create_controller(
    const KeyboardController::KeyboardControls& keys) {
    if ((keys.left == sf::Keyboard::Unknown) and
        (keys.right == sf::Keyboard::Unknown) and
        (keys.boost == sf::Keyboard::Unknown)) {
        return std::make_unique<MouseController>(getGame());
    } else if ((keys.left != sf::Keyboard::Unknown) and
               (keys.right != sf::Keyboard::Unknown) and
               (keys.boost != sf::Keyboard::Unknown)) {
        return std::make_unique<KeyboardController>(keys);
    } else
        throw std::runtime_error("Inconsistently set controller keys.");
}

Player* ArenaScene::add_player(const std::string& name, sf::Color primaryColor,
                              sf::Color secondaryColor,
                              std::unique_ptr<Controller> controller,
                              Viper* viper, sf::View view) {
    auto& player = _players.emplace_back(
        make_unique<Player>(name, std::move(controller), viper));
    player->setColors(primaryColor, secondaryColor);
    auto& panel = _player_panels.emplace_back(std::make_unique<PlayerPanel>(
        view, player.get(), getGame().getFontService()));
    viper->addObserver(panel.get(), {GameEvent::EventType::ObjectModified});
    player->addObserver(panel.get(), {GameEvent::EventType::ObjectModified});
    return player.get();
}

void ArenaScene::delete_player(Player* player) {
    _player_panels.erase(
        std::find_if(_player_panels.begin(), _player_panels.end(),
                     [player](std::unique_ptr<PlayerPanel>& panel) {
                         return panel->getPlayer() == player;
                     }));
    _players.erase(
        std::find_if(_players.begin(), _players.end(),
                     [player](std::unique_ptr<Player>& unique_player) {
                         return unique_player.get() == player;
                     }));
}

void ArenaScene::add_vipers(size_t number_of_vipers) {
    for (size_t i = 0; i < number_of_vipers; ++i) {
        auto& viper = _vipers.emplace_back(std::make_unique<Viper>(
            getGame().getOptionsService(), getGame().getTextureService()));
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
        _collision_manager.register_colliding_body(viper.get());
    }
}

void ArenaScene::kill_viper(Viper* viper) {
    // You cannot kill what's already dead
    if (viper->state() == GameObject::Alive)
        viper->state(GameObject::Dying);
}

void ArenaScene::delete_viper(Viper* viper) {
    _collision_manager.deregister_colliding_body(viper);
    _vipers.erase(std::find_if(_vipers.begin(), _vipers.end(),
                               [viper](std::unique_ptr<Viper>& unique_viper) {
                                   return unique_viper.get() == viper;
                               }));
}

void ArenaScene::add_food(Vec2 position, double diameter) {
    auto& food = _food.emplace_back(std::make_unique<Food>(
        position, diameter, timeFromSeconds(Random::getDouble(5, 10)),
        getGame().getColorService().getColor(Random::getInt())));
    // Check for collisions
    _collision_manager.register_colliding_body(food.get());
    food->addObserver(this, {GameEvent::EventType::Destroy});
}

void ArenaScene::delete_food(Food* food) {
    _collision_manager.deregister_colliding_body(food);
    _food.erase(std::find_if(_food.begin(), _food.end(),
                             [food](std::unique_ptr<Food>& unique_food) {
                                 return unique_food.get() == food;
                             }));
}

void ArenaScene::eat_food(Viper* viper, Food* food) {
    viper->eat(*food);
    food->state(GameObject::Dying);
    score_t score = food->score_value();
    auto player = find_player_with_viper(viper);
    player->score(score);

    PlayerPanel* panel = find_player_panel(player);
    auto& flyingScore =
        _flying_scores.emplace_back(std::make_unique<FlyingScore>(
            Vec2(getGame().getWindow().mapCoordsToPixel(food->getPosition(),
                                                        _game_view)),
            4 * viper->velocity(),
            Vec2(getGame().getWindow().mapCoordsToPixel(panel->getScoreTarget(),
                                                        panel->getView())),
            1s, score, getGame().getFontService()));
    flyingScore->setColor(sf::Color::Magenta, sf::Color::Red);
    flyingScore->setFontSize(0.03 * getGame().getWindow().getSize().y, 1.0);
    flyingScore->addObserver(this, {GameEvent::EventType::Destroy});
    flyingScore->addObserver(panel, {GameEvent::EventType::Scoring});
}

PlayerPanel* ArenaScene::find_player_panel(const Player* player) const {
    auto iter =
        std::find_if(_player_panels.begin(), _player_panels.end(),
                     [player](const std::unique_ptr<PlayerPanel>& panel) {
                         return panel->getPlayer() == player;
                     });
    if (iter == _player_panels.end())
        return nullptr;
    return iter->get();
}

Player* ArenaScene::findPlayerWith(const Controller* controller) const {
    auto iter =
        std::find_if(_players.begin(), _players.end(),
                     [controller](const std::unique_ptr<Player>& player) {
                         return player->controller() == controller;
                     });
    if (iter == _players.end())
        return nullptr;
    return iter->get();
}

Player* ArenaScene::find_player_with_viper(const Viper* viper) const {
    auto iter = std::find_if(_players.begin(), _players.end(),
                             [viper](const std::unique_ptr<Player>& player) {
                                 return player->viper() == viper;
                             });
    if (iter == _players.end())
        return nullptr;
    return iter->get();
}

Vec2 ArenaScene::find_free_circular_space(double radius) {
    auto game_area =
        sf::Rect<double>(0, 0, _game_view.getSize().x, _game_view.getSize().y);
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
    auto game_area_size = Vec2(_game_view.getSize().x, _game_view.getSize().y);
    const int max_attempts = 100000;
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

void ArenaScene::dispense_food() {
    while (_food.size() < 2) {
        double smallest = Food::nominalFoodRadius * 0.75;
        double largest = Food::nominalFoodRadius * 1.25;
        double food_radius = std::sqrt(
            Random::getDouble(smallest * smallest, largest * largest));
        // Find a spot, with some room to spare
        add_food(find_free_circular_space(2 * food_radius), food_radius);
    }
}

void ArenaScene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.clear(sf::Color::Black);
    for (const auto& panel : _player_panels) {
        target.setView(panel->getView());
        target.draw(*panel, states);
    }
    target.setView(_game_view);
    target.draw(*_walls, states);
    for (const auto& f : _food)
        target.draw(*f, states);
    for (const auto& v : _vipers)
        target.draw(*v, states);
    target.setView(target.getDefaultView());
    for (const auto& s : _flying_scores)
        target.draw(*s, states);
}

void ArenaScene::handle_collisions() {
    for (auto& collision : _collision_manager.check_for_collisions()) {
        handle_collision(collision);
    }
}

void ArenaScene::handle_collision(const CollisionPair& collision) {
    /* Only collisions involving a viper is interesting so both ColliderSegments
     * need to be checked in case they are a viper. If both are vipers we check
     * for consequences one viper at a time. */
    for (const auto& collider_segment : {collision.first, collision.second}) {
        // A collidee is what the collider collides into :)
        const auto collidee_segment =
            collider_segment.body == collision.first.body ? collision.second
                                                          : collision.first;
        // If it's not a viper, we don't care!
        const CollidingBody* collider_base_object = collider_segment.body;
        if (typeid(*collider_base_object) == typeid(Viper)) {
            Viper* collider_viper = (Viper*)&collider_base_object;
            if (collider_viper->state() == Viper::Alive &&
                collider_segment.index == 0) {
                if (typeid(*collidee_segment.body) == typeid(Food)) {
                    Food* food = (Food*)collidee_segment.body;
                    if (food->state() == GameObject::Alive)
                        eat_food(collider_viper, food);
                } else if (typeid(*collidee_segment.body) == typeid(Walls)) {
                    tagDebug("Killed by collision between Viper segment ",
                             collider_segment.index, " and wall segment ",
                             collidee_segment.index);
                    kill_viper(collider_viper);
                } else if (typeid(*collidee_segment.body) == typeid(Viper)) {
                    // Self collision
                    if (collider_viper == (Viper*)collidee_segment.body) {
                        auto segment_distance =
                            std::abs(int(collider_segment.index) -
                                     int(collidee_segment.index));
                        // Check if it's just neighbouring segments touching or
                        // a real collision
                        if (segment_distance > 1) {
                            tagDebug(
                                "Killed by collision between Viper segment ",
                                collider_segment.index, " and Viper segment ",
                                collidee_segment.index);
                            kill_viper(collider_viper);
                        }
                    }
                    // Collision with other viper
                    else
                        kill_viper(collider_viper);
                } else
                    throw std::runtime_error("Unknown collision happend");
            }
        }
    }
}

void ArenaScene::handle_steering() {
    for (auto& player : _players) {
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

void ArenaScene::handle_destruction(const DestroyEvent* event) {
    if (typeid(*event->objectPtr) == typeid(Viper))
        // we could retrieve the non-const ptr from the player but this is
        // easier
        delete_viper((Viper*)event->objectPtr);
    else if (typeid(*event->objectPtr) == typeid(Food))
        delete_food((Food*)event->objectPtr);
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

void ArenaScene::handle_object_updates(Time elapsedTime) {
    for (auto& v : _vipers)
        v->update(elapsedTime);
    for (auto& f : _food)
        f->update(elapsedTime);
    for (auto& s : _flying_scores)
        s->update(elapsedTime);
}

void ArenaScene::check_for_game_over() {
    for (auto& player : _players)
        if (player->viper() && player->viper()->state() == GameObject::Alive)
            return;  // Keep on playing as long as someone is alive

    setTransitionState(TransitionState::Spawn);
    // This scene must be kept alive since GameOverScreen reads
    std::vector<const Player*> players;
    for (auto& p : _players)
        players.push_back(p.get());
    _transition_scene = make_shared<GameOverScene>(getGame(), players);
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
                    _transition_scene = _pause_scene;
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

void ArenaScene::process_game_events() {
    auto [beginDestroyEvents, endDestroyEvents] =
        _events_to_be_processed.equal_range(GameEvent::EventType::Destroy);
    for (auto iter = beginDestroyEvents; iter != endDestroyEvents; ++iter)
        handle_destruction(static_cast<const DestroyEvent*>(iter->second));
    // Delete fully processed events?
    // For now just delete all...
    for (auto& event : _events_to_be_processed)
        delete event.second;
    _events_to_be_processed.clear();
}

void ArenaScene::update(Time elapsedTime) {
    handle_steering();
    Stopwatch clock;
    clock.start();
    handle_object_updates(elapsedTime);
    tagInfo("Game object updates took: ", clock.split());
    handle_collisions();
    tagInfo("Collision handling took: ", clock.split());
    dispense_food();
    process_game_events();
    check_for_game_over();
}

}  // namespace VVipers