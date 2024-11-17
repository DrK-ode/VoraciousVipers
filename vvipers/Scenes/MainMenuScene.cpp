#include <memory>
#include <vvipers/Scenes/ArenaScene.hpp>
#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Scenes/OptionsMenuScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>

namespace VVipers {

MainMenuScene::MainMenuScene(Game& game) : MenuScene(game) {
    auto size = game.window().getSize();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.25 * size), 0.5 * Vec2(size.x, size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(0.25, 0.25, 0.5, 0.5));
    setMenuView(menuView);

    _play_button = std::make_unique<MenuButton>();
    _play_button->setLabel("Play");
    _play_button->setFont(*game.font_service().default_font());
    addItem(_play_button.get());

    _options_button = std::make_unique<MenuButton>();
    _options_button->setLabel("Options");
    _options_button->setFont(*game.font_service().default_font());
    addItem(_options_button.get());

    _quit_button = std::make_unique<MenuButton>();
    _quit_button->setLabel("Quit");
    _quit_button->setFont(*game.font_service().default_font());
    addItem(_quit_button.get());

    distributeMenuItems();
    setSelectedIndex(0);
    setColors(sf::Color::Transparent, game.color_service().get_color(0),
              game.color_service().get_color(1));
}

void MainMenuScene::on_menu_item_activation(MenuItem* menuItem) {
    if (menuItem == _play_button.get()) {
        set_scene_state(SceneState::Paused);
        set_transition_state(TransitionState::Spawn);
        _transition_to = std::make_shared<ArenaScene>(game());
    } else if (menuItem == _options_button.get()) {
        set_scene_state(SceneState::Paused);
        set_transition_state(TransitionState::Spawn);
        _transition_to = std::make_shared<OptionsMenuScene>(game());
    } else if (menuItem == _quit_button.get()) {
        set_scene_state(SceneState::Paused);
        set_transition_state(TransitionState::Quit);
    }
}

std::shared_ptr<Scene> MainMenuScene::make_transition() { return _transition_to; }

}  // namespace VVipers