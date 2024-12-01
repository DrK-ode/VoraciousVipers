#include "vvipers/Scenes/PlayerConfScene.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <string>
#include <vector>

#include "vvipers/Engine/GameResources.hpp"
#include "vvipers/Engine/Scene.hpp"
#include "vvipers/GameElements/GameEvent.hpp"
#include "vvipers/UIElements/MenuButton.hpp"
#include "vvipers/UIElements/ToggleButton.hpp"

namespace VVipers {

PlayerConfScene::PlayerConfScene(GameResources& game)
    : MenuScene(game), _listening_for_key(nullptr) {
    auto size = game.window_manager().window_size();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.5 * 0.75 * size.x, 0.5 * 0.5 * size.y),
                      Vec2(.75 * size.x, .5 * size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(.125, .25, .75, .5));
    set_menu_view(menuView);

    size_t number_of_players =
        game.options_service().option_int("Players/numberOfPlayers");
    std::vector<size_t> player_numbers;
    for (size_t i = 1; i <= number_of_players; ++i)
        player_numbers.push_back(i);
    _player_button =
        std::make_unique<SelectionButton<size_t>>("Player: ", player_numbers);
    add_item(_player_button.get());

    _set_left_button = std::make_unique<MenuButton>();
    add_item(_set_left_button.get());

    _set_right_button = std::make_unique<MenuButton>();
    add_item(_set_right_button.get());

    _set_boost_button = std::make_unique<MenuButton>();
    add_item(_set_boost_button.get());

    _use_mouse_button = std::make_unique<ToggleButton>(
        "Mouse: on", "Mouse: off",
        game_resources().options_service().option_boolean(
            "Players/Player" +
            std::to_string(_player_button->selected_option()) + "/useMouse"));

    add_item(_use_mouse_button.get());

    _back_button = std::make_unique<MenuButton>();
    _back_button->set_label("Back");
    add_item(_back_button.get());

    set_draw_state(DrawState::Transparent);
    set_colors(sf::Color::Transparent, game.color_service().get_color(0));
    set_texts(*game.font_service().default_font(),
              game.color_service().get_color(1));
    set_selected_index(0);
    update_labels();
}

void PlayerConfScene::on_menu_item_activation(MenuItem* menu_item) {
    if (!menu_item->is_enabled()) {
        return;
    }
    if (menu_item == _set_left_button.get() ||
        menu_item == _set_right_button.get() ||
        menu_item == _set_boost_button.get()) {
        _listening_for_key = menu_item;
    } else if (menu_item == _use_mouse_button.get()) {
        _use_mouse_button->toggle();
        game_resources().options_service().set_option_boolean(
            "Players/Player" +
                std::to_string(_player_button->selected_option()) + "/useMouse",
            _use_mouse_button->is_toggled());
        _set_left_button->enable(!_use_mouse_button->is_toggled());
        _set_right_button->enable(!_use_mouse_button->is_toggled());
        _set_boost_button->enable(!_use_mouse_button->is_toggled());
    } else if (menu_item == _back_button.get()) {
        go_back();
    }
}

std::vector<int> PlayerConfScene::selected_player_keys() {
    return game_resources().options_service().option_int_array(
        "Players/Player" + std::to_string(_player_button->selected_option()) +
        "/keys");
}

void PlayerConfScene::set_selected_player_keys(const std::vector<int>& keys) {
    game_resources().options_service().set_option_int_array(
        "Players/Player" + std::to_string(_player_button->selected_option()) +
            "/keys",
        keys);
}

std::vector<std::string> PlayerConfScene::keys_to_strings(
    const std::vector<int>& scan_codes) const {
    std::vector<std::string> strings;
    for (auto scan_code : scan_codes) {
        auto key = sf::Keyboard::Scancode(scan_code);
        strings.push_back(key == sf::Keyboard::Scancode::Unknown
                              ? "unset"
                              : sf::Keyboard::getDescription(key));
    }
    return strings;
}

void PlayerConfScene::set_player_key(const MenuItem* menu_item,
                                     sf::Keyboard::Scancode scancode) {
    auto keys = selected_player_keys();
    size_t key_index = menu_item == _set_left_button.get()
                           ? 0
                           : (menu_item == _set_right_button.get() ? 1 : 2);
    keys[key_index] = scancode;
    set_selected_player_keys(keys);
    update_labels();
}

void PlayerConfScene::on_notify(const GameEvent& event) {
    if (run_state() != Scene::RunState::Running) {
        return;
    }
    if (_listening_for_key != nullptr) {
        if (event.type() == GameEvent::EventType::Keyboard) {
            const KeyboardEvent& keyboard_event =
                dynamic_cast<const KeyboardEvent&>(event);
            if (keyboard_event.keyboard_event_type ==
                KeyboardEvent::KeyboardEventType::KeyPressed) {
                set_player_key(_listening_for_key, keyboard_event.scancode);
                _listening_for_key = nullptr;
            }
        }
        return;
    }
    MenuScene::on_notify(event);
    switch (event.type()) {
        case GameEvent::EventType::ObjectModified: {
            const ObjectModifiedEvent& menu_event =
                dynamic_cast<const ObjectModifiedEvent&>(event);
            if (menu_event.object_pointer == _player_button.get()) {
                update_labels();
            }
            break;
        }
        case GameEvent::EventType::Keyboard: {
            const KeyboardEvent& keyboard_event =
                dynamic_cast<const KeyboardEvent&>(event);
            if (keyboard_event.keyboard_event_type ==
                KeyboardEvent::KeyboardEventType::KeyPressed) {
                switch (keyboard_event.scancode) {
                    case sf::Keyboard::Scan::Left: {
                        if (selected() == _player_button.get()) {
                            _player_button->option_left();
                        }
                        break;
                    }
                    case sf::Keyboard::Scan::Right: {
                        if (selected() == _player_button.get()) {
                            _player_button->option_right();
                        }
                        break;
                    }
                    case sf::Keyboard::Scan::Backspace: {
                        auto selected_menu_item = selected();
                        if (selected_menu_item == _set_left_button.get() ||
                            selected_menu_item == _set_right_button.get() ||
                            selected_menu_item == _set_boost_button.get()) {
                            set_player_key(selected_menu_item,
                                           sf::Keyboard::Scan::Unknown);
                        }
                    }
                    default:
                        break;
                }
            }
            break;
        }
        default:
            break;
    }
}

void PlayerConfScene::update_labels() {
    auto keys = selected_player_keys();
    auto key_strings = keys_to_strings(keys);

    _set_left_button->set_label("Left: " + key_strings[0]);
    _set_right_button->set_label("Right: " + key_strings[1]);
    _set_boost_button->set_label("Boost: " + key_strings[2]);

    _use_mouse_button->set_toggle_state(
        game_resources().options_service().option_boolean(
            "Players/Player" +
            std::to_string(_player_button->selected_option()) + "/useMouse"));

    _set_left_button->enable(!_use_mouse_button->is_toggled());
    _set_right_button->enable(!_use_mouse_button->is_toggled());
    _set_boost_button->enable(!_use_mouse_button->is_toggled());

    distribute_menu_items();
}

void PlayerConfScene::on_return() {
    MenuScene::on_return();
    game_resources().options_service().write();
}

}  // namespace VVipers