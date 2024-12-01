#pragma once

#include <memory>
#include <vvipers/Engine/GameResources.hpp>
#include <vvipers/UIElements/MenuScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>
#include "vvipers/GameElements/GameEvent.hpp"
#include "vvipers/UIElements/SelectionButton.hpp"

namespace VVipers {

class OptionsMenuScene : public MenuScene {
  public:
    OptionsMenuScene(GameResources& game);
    void on_menu_item_activation(MenuItem* menuItem) override;
    void on_notify(const GameEvent&) override;
    void on_return() override;

  private:
    std::unique_ptr<MenuButton> _back_button;
    std::unique_ptr<SelectionButton<size_t>> _players_button;
    std::unique_ptr<MenuButton> _player_conf_button;
    std::shared_ptr<Scene> _transition_to;
};

}  // namespace VVipers
