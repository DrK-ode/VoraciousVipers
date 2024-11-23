#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/PauseScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>
#include "vvipers/Engine/Scene.hpp"

namespace VVipers {

PauseScene::PauseScene(Game& game) : MenuScene(game) {
    Vec2 size = game.window().getSize();

    _background.setSize(size);
    _background.setFillColor(sf::Color(0,0,0,0x80));

    // Center and size in original coordinates
    Vec2 menu_relative_size(0.7, 0.2);
    sf::View menu_view(0.5 * menu_relative_size * size, menu_relative_size * size);
    // Relative position and size in screen coordinates
    menu_view.setViewport(
        sf::FloatRect(0.5 * (Vec2(1, 1) - menu_relative_size), menu_relative_size));
    setMenuView(menu_view);

    _continue_button = std::make_unique<MenuButton>();
    _continue_button->setLabel("Continue");
    _continue_button->setFont(*game.font_service().default_font());
    addItem(_continue_button.get());

    _quit_button = std::make_unique<MenuButton>();
    _quit_button->setLabel("Quit to main menu");
    _quit_button->setFont(*game.font_service().default_font());
    addItem(_quit_button.get());

    setSelectedIndex(0);
    distributeMenuItems();
    setColors(sf::Color::Transparent, game.color_service().get_color(0),
              game.color_service().get_color(1));

    set_draw_state(DrawState::Transparent);
}

void PauseScene::draw_background(sf::RenderTarget& target,
                                sf::RenderStates states) const {
    target.draw(_background, states);
}

void PauseScene::on_menu_item_activation(MenuItem* menu_item) {
    if (menu_item == _continue_button.get()) {
        set_run_state(RunState::Paused);
        set_transition_state(TransitionState::Return);
    } else if (menu_item == _quit_button.get()) {
        set_run_state(RunState::Paused);
        set_transition_state(TransitionState::Default);
    }
}

}  // namespace VVipers