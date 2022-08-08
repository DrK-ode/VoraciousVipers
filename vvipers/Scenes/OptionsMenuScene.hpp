#ifndef VVIPERS_SCENES_OPTIONSMENUSCENE_HPP
#define VVIPERS_SCENES_OPTIONSMENUSCENE_HPP

#include <memory>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/UIElements/MenuScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>

namespace VVipers {

class OptionsMenuScene : public MenuScene {
  public:
    OptionsMenuScene(Game& game);
    void onMenuItemActivation(MenuItem* menuItem) override;

  private:
    std::unique_ptr<MenuButton> m_backButton;
};

}  // namespace VVipers

#endif // VVIPERS_SCENES_OPTIONSMENUSCENE_HPP
