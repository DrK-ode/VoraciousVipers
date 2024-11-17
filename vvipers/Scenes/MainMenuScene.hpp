#pragma once

#include <memory>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/UIElements/MenuButton.hpp>
#include <vvipers/UIElements/MenuScene.hpp>

namespace VVipers {

class MainMenuScene : public MenuScene {
  public:
    MainMenuScene(Game& game);
    void on_menu_item_activation(MenuItem* menuItem) override;
    std::shared_ptr<Scene> make_transition() override;

  private:
    std::unique_ptr<MenuButton> _play_button;
    std::unique_ptr<MenuButton> _options_button;
    std::unique_ptr<MenuButton> _quit_button;
    std::shared_ptr<Scene> _transition_to;
};

}  // namespace VVipers
