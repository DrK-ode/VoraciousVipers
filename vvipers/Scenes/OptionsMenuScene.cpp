#include <vvipers/Scenes/OptionsMenuScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>
#include "vvipers/Engine/Scene.hpp"

namespace VVipers
{

OptionsMenuScene::OptionsMenuScene(Game& game) : MenuScene(game) {
    auto size = game.window().getSize();
    // Center and size in original coordinates
    sf::View menuView(Vec2(0.25 * size), 0.5 * Vec2(size.x, size.y));
    // Relative position and size in screen coordinates
    menuView.setViewport(sf::FloatRect(0.25, 0.25, 0.5, 0.5));
    setMenuView(menuView);

    _players_button = std::make_unique<MenuButton>();
    _players_button->set_label("Player count: 0");
    _players_button->set_font( *game.font_service().default_font() );
    addItem(_players_button.get());

    _player_conf_button = std::make_unique<MenuButton>();
    _player_conf_button->set_label("Player config");
    _player_conf_button->set_font( *game.font_service().default_font() );
    addItem(_player_conf_button.get());

    _back_button = std::make_unique<MenuButton>();
    _back_button->set_label("Back");
    _back_button->set_font( *game.font_service().default_font() );
    addItem(_back_button.get());

    setSelectedIndex(0);
    distributeMenuItems();
    set_draw_state(DrawState::Transparent);
    setColors(sf::Color::Transparent, game.color_service().get_color(0),
              game.color_service().get_color(1));
}

void OptionsMenuScene::on_menu_item_activation(MenuItem* menuItem){
    if( menuItem == _back_button.get()){
        set_run_state(RunState::Paused);
        set_transition_state(TransitionState::Return );
    }
}

} // namespace VVipers