#ifndef VVIPERS_SCENES_MAINMENUSCENE_HPP
#define VVIPERS_SCENES_MAINMENUSCENE_HPP

#include <memory>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/MenuScene.hpp>
#include <vvipers/Scenes/UIElements/MenuButton.hpp>

namespace VVipers {

class MainMenuScene : public MenuScene {
  public:
    MainMenuScene(Game& game);
    void onMenuItemActivation(MenuItem* menuItem) override;
    scene_ptr makeTransition() override;

  private:
    std::unique_ptr<MenuButton> m_playButton;
    std::unique_ptr<MenuButton> m_quitButton;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_MAINMENUSCENE_HPP
