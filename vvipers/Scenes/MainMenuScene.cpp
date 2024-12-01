#include <memory>
#include <vvipers/Scenes/ArenaScene.hpp>
#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Scenes/OptionsMenuScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>
#include "vvipers/GameElements/GameEvent.hpp"
#include "vvipers/UIElements/MenuScene.hpp"

namespace VVipers {

MainMenuScene::MainMenuScene(GameResources& game) : MenuScene(game) {
    auto size = game.window_manager().window_size();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.25 * size), 0.5 * Vec2(size.x, size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(0.25, 0.25, 0.5, 0.5));
    set_menu_view(menuView);

    _play_button = std::make_unique<MenuButton>();
    _play_button->set_label("Play");
    _play_button->set_font(*game.font_service().default_font());
    add_item(_play_button.get());

    _options_button = std::make_unique<MenuButton>();
    _options_button->set_label("Options");
    _options_button->set_font(*game.font_service().default_font());
    add_item(_options_button.get());

    _quit_button = std::make_unique<MenuButton>();
    _quit_button->set_label("Quit");
    _quit_button->set_font(*game.font_service().default_font());
    add_item(_quit_button.get());

    distribute_menu_items();
    set_selected_index(0);
    set_colors(sf::Color::Transparent, game.color_service().get_color(0),
              game.color_service().get_color(1));
}

void MainMenuScene::on_menu_item_activation(MenuItem* menuItem) {
    if (menuItem == _play_button.get()) {
        set_run_state(RunState::Paused);
        set_draw_state(DrawState::Skip);
        SceneEvent scene_event(SceneEvent::SceneEventType::Spawn);
        scene_event.target_scene = std::make_shared<ArenaScene>(game_resources());
        notify(scene_event);
    } else if (menuItem == _options_button.get()) {
        set_run_state(RunState::Paused);
        set_draw_state(DrawState::Skip);
        SceneEvent scene_event(SceneEvent::SceneEventType::Spawn);
        scene_event.target_scene = std::make_shared<OptionsMenuScene>(game_resources());
        notify(scene_event);
    } else if (menuItem == _quit_button.get()) {
        set_run_state(RunState::Paused);
        SceneEvent scene_event(SceneEvent::SceneEventType::Quit);
        notify(scene_event);
    }
}

void MainMenuScene::on_activation(){
    MenuScene::on_activation();
    set_draw_state(DrawState::Solid);
}


}  // namespace VVipers