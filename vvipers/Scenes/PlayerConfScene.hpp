#pragma once

#include <memory>

#include "vvipers/Engine/GameResources.hpp"
#include "vvipers/UIElements/MenuButton.hpp"
#include "vvipers/UIElements/MenuScene.hpp"
#include "vvipers/UIElements/SelectionButton.hpp"
#include "vvipers/UIElements/ToggleButton.hpp"

namespace VVipers {

class PlayerConfScene : public MenuScene {
  public:
    PlayerConfScene(GameResources& game);
    void on_menu_item_activation(MenuItem* menuItem) override;
    void on_notify(const GameEvent& event) override;
    void on_return() override;

  private:
    std::vector<std::string> keys_to_strings(const std::vector<int>&) const;
    std::vector<int> selected_player_keys();
    void set_selected_player_keys(const std::vector<int>& keys);
    void set_player_key(const MenuItem*, sf::Keyboard::Scancode);
    void update_labels();

    std::unique_ptr<SelectionButton<size_t>> _player_button;
    std::unique_ptr<MenuButton> _set_left_button;
    std::unique_ptr<MenuButton> _set_right_button;
    std::unique_ptr<MenuButton> _set_boost_button;
    std::unique_ptr<ToggleButton> _use_mouse_button;
    std::unique_ptr<MenuButton> _back_button;
    std::shared_ptr<Scene> _transition_to;

    const MenuItem* _listening_for_key;
};

}  // namespace VVipers
