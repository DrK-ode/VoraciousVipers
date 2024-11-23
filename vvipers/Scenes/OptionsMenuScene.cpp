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

    _back_button = std::make_unique<MenuButton>();
    _back_button->setLabel("Back");
    _back_button->setFont( *game.font_service().default_font() );
    addItem(_back_button.get());

    setSelectedIndex(0);
    distributeMenuItems();
    set_draw_state(DrawState::Transparent);
}

void OptionsMenuScene::on_menu_item_activation(MenuItem* menuItem){
    if( menuItem == _back_button.get()){
        set_run_state(RunState::Paused);
        set_transition_state(TransitionState::Return );
    }
}

} // namespace VVipers