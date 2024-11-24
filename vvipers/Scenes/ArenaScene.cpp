#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
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

#include "vvipers/GameElements/GameObject.hpp"
#include "vvipers/UIElements/PlayerPanel.hpp"
#include "vvipers/Utilities/Time.hpp"
#include "vvipers/Utilities/VVMath.hpp"

namespace VVipers {

using namespace std::chrono_literals;

ArenaScene::PlayerData ArenaScene::read_player_conf(size_t player) {
    std::stringstream ss;
    ss << "Players/Player" << player + 1 << "/";
    std::string base_name = ss.str();

    auto name =
        game().options_service().option_string(base_name + std::string("name"));
    auto color1 = color_from_rgb_string(
        game().options_service().option_string(base_name + "primaryColor"));
    auto color2 = color_from_rgb_string(
        game().options_service().option_string(base_name + "secondaryColor"));
    auto keys = game().options_service().option_int_array(base_name + "keys");
    if (keys.size() != 3)
        throw std::runtime_error(
            "Wrong number of keys in player configuration.");
    return {name, color1, color2, keys};
}

ArenaScene::ArenaScene(Game& game) : Scene(game), _collision_manager(5, 100.) {
    size_t number_of_players =
        game.options_service().option_int("Players/numberOfPlayers");
    std::vector<PlayerData> player_data;
    for (size_t player = 0; player < number_of_players; ++player) {
        player_data.push_back(read_player_conf(player));
    }

    auto numberOfDivisions = number_of_players + 1;
    Vec2 window_size = game.window().getSize();
    const sf::Vector2f status_bar_relative_size(1. / numberOfDivisions, 0.1);
    const sf::Vector2f status_bar_size(
        window_size.x * status_bar_relative_size.x,
        window_size.y * status_bar_relative_size.y);

    std::vector<sf::View> status_bar_views;
    status_bar_views.resize(numberOfDivisions);
    for (size_t i = 0; i < numberOfDivisions; ++i) {
        status_bar_views[i].setSize(status_bar_size);
        status_bar_views[i].setCenter(0.5 * status_bar_size.x,
                                      0.5 * status_bar_size.y);
        status_bar_views[i].setViewport(sf::FloatRect(
            {i * status_bar_relative_size.x, 0.}, status_bar_relative_size));
    }

    const sf::Vector2f game_relative_size(1, 1 - status_bar_relative_size.y);
    const sf::Vector2f game_size(window_size.x * game_relative_size.x,
                                 window_size.y * game_relative_size.y);
    _game_view.setSize(game_size);
    _game_view.setCenter(game_size / 2);
    _game_view.setViewport(
        sf::FloatRect({0.f, status_bar_relative_size.y}, game_relative_size));

    _walls = std::make_unique<Walls>(game_size);
    _collision_manager.register_colliding_body(_walls.get());

    // The players must absolutely be added _after_ the level has been filled
    // with obstacles, otherwise they might end up inside or on top of them.
    dispense_food();
    add_players(player_data, status_bar_views);

    // Only create one pause screen so that it can be reused
    _pause_scene = std::make_shared<PauseScene>(game);
}

ArenaScene::~ArenaScene() {}

void ArenaScene::add_players(std::vector<PlayerData>& player_data,
                             std::vector<sf::View>& playerViews) {
    auto numberOfPlayers = player_data.size();
    // Vipers are added to the member vector _vipers
    for (int i = 0; i < numberOfPlayers; ++i) {
        KeyboardController::KeyboardControls keys;
        keys.left = sf::Keyboard::Scancode(player_data[i].keys[0]);
        keys.right = sf::Keyboard::Scancode(player_data[i].keys[1]);
        keys.boost = sf::Keyboard::Scancode(player_data[i].keys[2]);
        auto controller = create_controller(keys);
        add_player(player_data[i], std::move(controller), add_viper(),
                   playerViews[i]);
    }
}

std::unique_ptr<Controller> ArenaScene::create_controller(
    const KeyboardController::KeyboardControls& keys) {
    if ((keys.left == sf::Keyboard::Scan::Unknown) and
        (keys.right == sf::Keyboard::Scan::Unknown) and
        (keys.boost == sf::Keyboard::Scan::Unknown)) {
        return std::make_unique<MouseController>(game());
    } else if ((keys.left != sf::Keyboard::Scan::Unknown) and
               (keys.right != sf::Keyboard::Scan::Unknown) and
               (keys.boost != sf::Keyboard::Scan::Unknown)) {
        return std::make_unique<KeyboardController>(keys);
    } else
        throw std::runtime_error("Inconsistently set controller keys.");
}

Player* ArenaScene::add_player(const PlayerData& data,
                               std::unique_ptr<Controller> controller,
                               std::shared_ptr<Viper> viper, sf::View view) {
    auto& player = _players.emplace_back(
        make_unique<Player>(data.name, std::move(controller), viper));
    player->set_colors(data.primary_color, data.secondary_color);
    auto& panel = _player_panels.emplace_back(std::make_unique<PlayerPanel>(
        view, player.get(), game().font_service()));
    viper->add_observer(panel.get(), {GameEvent::EventType::ObjectModified});
    player->add_observer(panel.get(), {GameEvent::EventType::ObjectModified});

    std::stringstream ss;
    ss << data.name << "'s viper";
    viper->set_name(ss.str());

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

std::shared_ptr<Viper> ArenaScene::add_viper() {
    static std::vector<Polygon> starting_areas;
    auto viper = std::make_shared<Viper>(game().options_service(),
                                         game().texture_service());
    double length = viper->speed() * 5;  // 5 seconds free space
    double width = viper->nominal_width();
    // Give some margin to the width
    Polygon starting_area(Vec2(length, 1.5 * width));
    starting_area.set_anchor(Vec2(-0.5 * length, 0));
    find_free_space_for(starting_area, true, starting_areas);
    starting_areas.push_back(starting_area);

    viper->setup(starting_area.anchor(), starting_area.angle(), 0.5);

    viper->add_observer(this, {GameEvent::EventType::Destroy});
    _collision_manager.register_colliding_body(viper.get());
    _vipers.push_back(viper);
    return viper;
}

void ArenaScene::kill_viper(Viper* viper) {
    // You cannot kill what's already dead
    if (viper->state() == GameObject::Alive)
        viper->state(GameObject::Dying);
}

void ArenaScene::delete_viper(Viper* viper) {
    _collision_manager.deregister_colliding_body(viper);
    _vipers.erase(std::find_if(_vipers.begin(), _vipers.end(),
                               [viper](std::shared_ptr<Viper>& unique_viper) {
                                   return unique_viper.get() == viper;
                               }));
}

void ArenaScene::add_food(Vec2 position, double diameter) {
    auto& food = _food.emplace_back(std::make_unique<Food>(
        position, diameter, time_from_seconds(Random::random_double(5, 10)),
        game().color_service().get_color(Random::random_int())));
    // Check for collisions
    _collision_manager.register_colliding_body(food.get());
    food->add_observer(this, {GameEvent::EventType::Destroy});
}

void ArenaScene::delete_food(Food* food) {
    _collision_manager.deregister_colliding_body(food);
    _food.erase(std::find_if(_food.begin(), _food.end(),
                             [food](std::unique_ptr<Food>& unique_food) {
                                 return unique_food.get() == food;
                             }));
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

Player* ArenaScene::find_player_with(const Controller* controller) const {
    auto iter =
        std::find_if(_players.begin(), _players.end(),
                     [controller](const std::unique_ptr<Player>& player) {
                         return player->controller() == controller;
                     });
    if (iter == _players.end())
        return nullptr;
    return iter->get();
}

Player* ArenaScene::find_player_with(const Viper* viper) const {
    auto iter = std::find_if(_players.begin(), _players.end(),
                             [viper](const std::unique_ptr<Player>& player) {
                                 return player->viper() == viper;
                             });
    if (iter == _players.end())
        return nullptr;
    return iter->get();
}

void ArenaScene::find_free_space_for(
    Shape& shape, bool allow_rotation,
    const std::vector<Polygon>& exclusion_zones) const {
    auto game_area =
        sf::Rect<double>(0, 0, _game_view.getSize().x, _game_view.getSize().y);
    const int max_attempts = 1000000;
    for (int i = 0; i < max_attempts; ++i) {
        Vec2 center(Random::random_double(game_area.left, game_area.width),
                    Random::random_double(game_area.top, game_area.height));
        shape.move_to(center);
        if (allow_rotation)
            shape.rotate(Random::random_double(0, twopi));
        bool excluded = false;
        for (auto& zone : exclusion_zones) {
            if (zone.overlap(shape)) {
                excluded = true;
                break;
            }
        }
        if (excluded) {
            continue;
        }
        if (!_collision_manager.is_occupied(shape))
            return;
    }
    throw std::runtime_error("Could not find an empty area.");
}

void ArenaScene::dispense_food() {
    while (_food.size() < 2) {
        double smallest = Food::nominalFoodRadius * 0.75;
        double largest = Food::nominalFoodRadius * 1.25;
        double food_radius = std::sqrt(
            Random::random_double(smallest * smallest, largest * largest));
        // Find a spot, with some room to spare
        Circle placeholder(2 * food_radius);
        find_free_space_for(placeholder);
        add_food(placeholder.center(), food_radius);
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
    BoundingBox the_world(_game_view.getCenter(), _game_view.getSize());
    for (auto& collision : _collision_manager.check_for_collisions(the_world)) {
        handle_collision(collision);
    }
}

void ArenaScene::handle_collision(const CollisionPair& collision) {
    /*tagDebug(collision.first.body->name(), "[", collision.first.index,
             "] collided with ", collision.second.body->name(), "[",
             collision.second.index, "].");*/
    for (const auto& collider : {collision.first, collision.second}) {
        // A collidee is what the collider collides into :)
        const auto& collidee = collider.body == collision.first.body
                                   ? collision.second
                                   : collision.first;
        // If it's not a viper, we don't care!
        if (typeid(*collider.body) == typeid(Viper)) {
            Viper* collider_viper = (Viper*)collider.body;
            if (typeid(*collidee.body) == typeid(Food)) {
                handle_viper_food_collision(collider_viper, collider.index,
                                            (Food*)collidee.body,
                                            collidee.index);
            } else if (typeid(*collidee.body) == typeid(Walls)) {
                handle_viper_walls_collision(collider_viper, collider.index,
                                             (Walls*)collidee.body,
                                             collidee.index);
            } else if (typeid(*collidee.body) == typeid(Viper)) {
                Viper* collidee_viper = (Viper*)collidee.body;
                handle_viper_viper_collision(collider_viper, collider.index,
                                             collidee_viper, collidee.index);
            } else
                throw std::runtime_error("Unknown collision happend");
        }
    }
}

void ArenaScene::handle_viper_food_collision(Viper* viper,
                                             size_t viper_segment_index,
                                             Food* food,
                                             size_t food_segment_index) {
    if (viper->state() != GameObject::Alive || viper_segment_index != 0 ||
        food->state() != GameObject::Alive)
        return;

    viper->eat(*food);
    food->state(GameObject::Dying);
    score_t score = food->score_value();
    auto player = find_player_with(viper);
    player->score(score);

    PlayerPanel* panel = find_player_panel(player);
    auto& flyingScore =
        _flying_scores.emplace_back(std::make_unique<FlyingScore>(
            Vec2(game().window().mapCoordsToPixel(food->getPosition(),
                                                  _game_view)),
            4 * viper->velocity(),
            Vec2(game().window().mapCoordsToPixel(panel->getScoreTarget(),
                                                  panel->getView())),
            1s, score, game().font_service()));
    flyingScore->setColor(sf::Color::Magenta, sf::Color::Red);
    flyingScore->setFontSize(0.03 * game().window().getSize().y, 1.0);
    flyingScore->add_observer(this, {GameEvent::EventType::Destroy});
    flyingScore->add_observer(panel, {GameEvent::EventType::Scoring});
}

void ArenaScene::handle_viper_viper_collision(Viper* collider_viper,
                                              size_t collider_segment_index,
                                              Viper* collidee_viper,
                                              size_t collidee_segment_index) {
    if (collider_viper->state() != GameObject::Alive ||
        collider_segment_index != 0)
        return;

    if (collider_viper == collidee_viper) {
        auto segment_distance =
            std::abs(int(collider_segment_index) - int(collidee_segment_index));
        // Check if it's just neighbouring segments touching or
        // a real collision
        if (segment_distance > 1) {
            log_debug(collider_viper->name(),
                      " was killed by a collision with ",
                      collidee_viper->name(), "[", collidee_segment_index, "]");
            kill_viper(collider_viper);
        }
    }
    // Collision with other viper
    else
        kill_viper(collider_viper);
}

void ArenaScene::handle_viper_walls_collision(Viper* viper,
                                              size_t viper_segment_index,
                                              Walls* walls,
                                              size_t walls_segment_index) {
    if (viper->state() != Viper::Alive || viper_segment_index != 0)
        return;

    log_debug(viper->name(), " was killed by a collision with ", walls->name(),
              "[", walls_segment_index, "]");
    kill_viper(viper);
}

void ArenaScene::handle_steering() {
    for (auto& player : _players) {
        if (auto controller = player->controller()) {
            SteeringCommand cmd = controller->control();

            if (Viper* viper = player->viper()) {
                // Protect against erroneous input from controller
                if (std::abs(cmd.turn) > 1)
                    cmd.turn /= std::abs(cmd.turn);
                double angularSpeed = cmd.turn * viper->max_angular_speed();
                /* Boost if the steering event says so and either:
                 *    1) Boost in inactive and the boost power is full
                 *    2) Boost is active and the boost power is not depleted
                 */
                double boost = 0.;
                if (cmd.boost &&
                    ((viper->boost_amount() > 0 and
                      viper->boost_charge() > time_from_seconds(0)) or
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

    set_transition_state(TransitionState::Spawn);
    // This scene must be kept alive since GameOverScreen reads
    std::vector<const Player*> players;
    for (auto& p : _players)
        players.push_back(p.get());
    _transition_scene = make_shared<GameOverScene>(game(), players);
}

void ArenaScene::on_notify(const GameEvent* event) {
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

void ArenaScene::process_event(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed: {
            set_run_state(RunState::Paused);
            set_transition_state(TransitionState::Quit);
            break;
        }
        case sf::Event::Resized: {
            break;
        }
        case sf::Event::KeyPressed: {
            switch (event.key.scancode) {
                case sf::Keyboard::Scan::Escape: {
                    set_run_state(RunState::Paused);
                    set_transition_state(TransitionState::Spawn);
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
    log_info("  Game object updates took: ", clock.split());
    handle_collisions();
    log_info("  Collision handling took: ", clock.split());
    dispense_food();
    process_game_events();
    check_for_game_over();
}

}  // namespace VVipers