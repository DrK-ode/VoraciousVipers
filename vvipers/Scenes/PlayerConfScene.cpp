#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <vector>
#include <vvipers/Scenes/PlayerConfScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>

#include "vvipers/Engine/Game.hpp"
#include "vvipers/Engine/Scene.hpp"
#include "vvipers/UIElements/ToggleButton.hpp"

namespace VVipers {

PlayerConfScene::PlayerConfScene(Game& game) : MenuScene(game) {
    auto size = game.window().getSize();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.5 * 0.75 * size.x, 0.5 * 0.5 * size.y),
                      Vec2(.75 * size.x, .5 * size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(.125, .25, .75, .5));
    set_menu_view(menuView);

    std::vector<size_t> player_numbers = {1, 2, 3, 4};
    _player_button =
        std::make_unique<SelectionButton<size_t>>("Player: ", player_numbers);
    _player_button->set_font(*game.font_service().default_font());
    add_item(_player_button.get());
    _player_button->add_observer(this, {GameEvent::EventType::Menu});

    _set_left_button = std::make_unique<MenuButton>();
    _set_left_button->set_font(*game.font_service().default_font());
    add_item(_set_left_button.get());

    _set_right_button = std::make_unique<MenuButton>();
    _set_right_button->set_font(*game.font_service().default_font());
    add_item(_set_right_button.get());

    _set_boost_button = std::make_unique<MenuButton>();
    _set_boost_button->set_font(*game.font_service().default_font());
    add_item(_set_boost_button.get());

    _enable_mouse_button = std::make_unique<ToggleButton>("Mouse: enabled", "Mouse: disabled", false);
    _enable_mouse_button->set_font(*game.font_service().default_font());
    add_item(_enable_mouse_button.get());
    _enable_mouse_button->add_observer(this, {GameEvent::EventType::Menu});

    _back_button = std::make_unique<MenuButton>();
    _back_button->set_label("Back");
    _back_button->set_font(*game.font_service().default_font());
    add_item(_back_button.get());

    update_labels();

    set_selected_index(0);
    distribute_menu_items();
    set_draw_state(DrawState::Transparent);
    set_colors(sf::Color::Transparent, game.color_service().get_color(0),
              game.color_service().get_color(1));
}

void PlayerConfScene::on_menu_item_activation(MenuItem* menuItem) {
    if (menuItem == _set_left_button.get()) {
        /*set_run_state(RunState::Paused);
        set_transition_state(TransitionState::Spawn );
        _transition_to = std::make_shared<SetKeysScene>();*/
    } else if (menuItem == _enable_mouse_button.get()) {
        _enable_mouse_button->toggle();
    } else if (menuItem == _back_button.get()) {
        set_run_state(RunState::Paused);
        set_transition_state(TransitionState::Return);
    }
}

std::shared_ptr<Scene> PlayerConfScene::make_transition() {
    return _transition_to;
}

std::vector<int> PlayerConfScene::selected_player_keys() {
    return game().options_service().option_int_array(
        "Players/Player" + std::to_string(_player_button->selected_option()) +
        "/keys");
}

std::vector<std::string> PlayerConfScene::keys_to_strings(
    const std::vector<int>& scan_codes) const {
    std::vector<std::string> strings;
    for (auto scan_code : scan_codes) {
        auto key = sf::Keyboard::Scancode(scan_code);
        strings.push_back(sf::Keyboard::getDescription(key));
    }
    return strings;
}

void PlayerConfScene::on_notify(const GameEvent& event) {
    if (event.type() == GameEvent::EventType::Menu) {
        const MenuEvent& menu_event = dynamic_cast<const MenuEvent&>(event);
        if (menu_event.sender == _player_button.get()) {
            update_labels();
        }
    }
}

void PlayerConfScene::update_labels() {
    auto keys = selected_player_keys();
    auto key_strings = keys_to_strings(keys);

    _set_left_button->set_label("Left: " + key_strings[0]);
    _set_right_button->set_label("Right: " + key_strings[1]);
    _set_boost_button->set_label("Boost: " + key_strings[2]);

    distribute_menu_items();
}

void PlayerConfScene::on_return() {
    set_run_state(RunState::Paused);
    set_transition_state(TransitionState::Return);
    game().options_service().write();
}

}  // namespace VVipers