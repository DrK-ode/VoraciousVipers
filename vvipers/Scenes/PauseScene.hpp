#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include <vvipers/UIElements/MenuButton.hpp>
#include <vvipers/UIElements/MenuScene.hpp>

namespace VVipers {

class Game;

class PauseScene : public MenuScene {
  public:
    PauseScene(Game& game);
    void draw_background(sf::RenderTarget& target,
                         sf::RenderStates states) const override;
    void on_menu_item_activation(MenuItem* menuItem) override;

  private:
    std::unique_ptr<MenuButton> _continue_button;
    std::unique_ptr<MenuButton> _quit_button;
    sf::RectangleShape _background;
};

}  // namespace VVipers
