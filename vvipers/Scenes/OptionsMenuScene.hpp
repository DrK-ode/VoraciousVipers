#pragma once

#include <memory>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/UIElements/MenuScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>

namespace VVipers {

class OptionsMenuScene : public MenuScene {
  public:
    OptionsMenuScene(Game& game);
    void on_menu_item_activation(MenuItem* menuItem) override;

  private:
    std::unique_ptr<MenuButton> _back_button;
};

}  // namespace VVipers
