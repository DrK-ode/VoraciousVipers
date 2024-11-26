#include <memory>
#include <vvipers/Scenes/OptionsMenuScene.hpp>
#include <vvipers/Scenes/PlayerConfScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>

#include "vvipers/Engine/Scene.hpp"
#include "vvipers/GameElements/GameEvent.hpp"

namespace VVipers {

OptionsMenuScene::OptionsMenuScene(Game& game) : MenuScene(game) {
    auto size = game.window().getSize();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.5 * 0.75 * size.x, 0.5 * 0.5 * size.y),
                      Vec2(.75 * size.x, .5 * size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(.125, .25, .75, .5));
    set_menu_view(menuView);

    std::vector<size_t> players = {1, 2, 3, 4};
    _players_button =
        std::make_unique<SelectionButton<size_t>>("Player count: ", players);
    _players_button->set_font(*game.font_service().default_font());
    add_item(_players_button.get());
    _players_button->set_selected_option(
        game.options_service().option_int("Players/numberOfPlayers") - 1);
    _players_button->add_observer(this, {GameEvent::EventType::Menu});

    _player_conf_button = std::make_unique<MenuButton>();
    _player_conf_button->set_label("Player config");
    _player_conf_button->set_font(*game.font_service().default_font());
    add_item(_player_conf_button.get());

    _back_button = std::make_unique<MenuButton>();
    _back_button->set_label("Back");
    _back_button->set_font(*game.font_service().default_font());
    add_item(_back_button.get());

    set_selected_index(0);
    distribute_menu_items();
    set_draw_state(DrawState::Transparent);
    set_colors(sf::Color::Transparent, game.color_service().get_color(0),
              game.color_service().get_color(1));
}

void OptionsMenuScene::on_menu_item_activation(MenuItem* menuItem) {
    if (menuItem == _player_conf_button.get()) {
        set_run_state(RunState::Paused);
        set_draw_state(DrawState::Skip);
        set_transition_state(TransitionState::Spawn);
        _transition_to = std::make_shared<PlayerConfScene>(game());
    } else if (menuItem == _back_button.get()) {
        on_return();
    }
}

std::shared_ptr<Scene> OptionsMenuScene::make_transition() {
    return _transition_to;
}

void OptionsMenuScene::on_activation() {
    MenuScene::on_activation();
    set_draw_state(DrawState::Transparent);
}

void OptionsMenuScene::on_notify(const GameEvent& event) {
    if (event.type() == GameEvent::EventType::Menu) {
        const MenuEvent& menu_event = dynamic_cast<const MenuEvent&>(event);
        if (menu_event.sender == _players_button.get()) {
            game().options_service().set_option_int(
                "Players/numberOfPlayers", _players_button->selected_option());
        }
    }
}

void OptionsMenuScene::on_return() {
    set_run_state(RunState::Paused);
    set_transition_state(TransitionState::Return);
    game().options_service().write();
}

}  // namespace VVipers