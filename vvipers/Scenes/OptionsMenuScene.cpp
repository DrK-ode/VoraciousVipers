#include "vvipers/Scenes/OptionsMenuScene.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include "vvipers/Engine/Scene.hpp"
#include "vvipers/GameElements/GameEvent.hpp"
#include "vvipers/Scenes/PlayerConfScene.hpp"
#include "vvipers/UIElements/MenuButton.hpp"
#include "vvipers/UIElements/MenuScene.hpp"

namespace VVipers {

OptionsMenuScene::OptionsMenuScene(GameResources& game) : MenuScene(game) {
    auto size = game.window_manager().window_size();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.5 * 0.75 * size.x, 0.5 * 0.5 * size.y),
                      Vec2(.75 * size.x, .5 * size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(.125, .25, .75, .5));
    set_menu_view(menuView);

    std::vector<size_t> players = {1, 2, 3, 4};
    _players_button =
        std::make_unique<SelectionButton<size_t>>("Player count: ", players);
    add_item(_players_button.get());
    _players_button->set_selected_option(
        game.options_service().option_int("Players/numberOfPlayers") - 1);

    _player_conf_button = std::make_unique<MenuButton>();
    _player_conf_button->set_label("Player config");
    add_item(_player_conf_button.get());

    _back_button = std::make_unique<MenuButton>();
    _back_button->set_label("Back");
    add_item(_back_button.get());

    set_selected_index(0);
    distribute_menu_items();
    set_draw_state(DrawState::Transparent);
    set_colors(sf::Color::Transparent, game.color_service().get_color(0));
    set_texts(*game.font_service().default_font(),
              game.color_service().get_color(1));
}

void OptionsMenuScene::on_menu_item_activation(MenuItem* menuItem) {
    if (menuItem == _player_conf_button.get()) {
        set_run_state(RunState::Paused);
        set_draw_state(DrawState::Skip);
        SceneEvent scene_event(SceneEvent::SceneEventType::Spawn);
        scene_event.target_scene =
            std::make_shared<PlayerConfScene>(game_resources());
        notify(scene_event);
    } else if (menuItem == _back_button.get()) {
        notify(SceneEvent(SceneEvent::SceneEventType::Return));
    }
}

void OptionsMenuScene::on_activation() {
    MenuScene::on_activation();
    set_draw_state(DrawState::Transparent);
}

void OptionsMenuScene::on_notify(const GameEvent& event) {
    if (run_state() != Scene::RunState::Running) {
        return;
    }
    MenuScene::on_notify(event);
    switch (event.type()) {
        case GameEvent::EventType::ObjectModified: {
            const ObjectModifiedEvent& object_modified_event =
                dynamic_cast<const ObjectModifiedEvent&>(event);
            if (object_modified_event.object_pointer == _players_button.get()) {
                game_resources().options_service().set_option_int(
                    "Players/numberOfPlayers",
                    _players_button->selected_option());
            }
            break;
        }
        case GameEvent::EventType::Keyboard: {
            const KeyboardEvent& keyboard_event =
                dynamic_cast<const KeyboardEvent&>(event);
            if (keyboard_event.keyboard_event_type ==
                    KeyboardEvent::KeyboardEventType::KeyPressed &&
                selected() == _players_button.get()) {
                switch (keyboard_event.scancode) {
                    case sf::Keyboard::Scan::Left: {
                        _players_button->option_left();
                        break;
                    }
                    case sf::Keyboard::Scan::Right: {
                        _players_button->option_right();
                        break;
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

void OptionsMenuScene::on_return() {
    game_resources().options_service().write();
}

}  // namespace VVipers