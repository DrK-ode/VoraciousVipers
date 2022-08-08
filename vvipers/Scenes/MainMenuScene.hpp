#ifndef VVIPERS_SCENES_MAINMENUSCENE_HPP
#define VVIPERS_SCENES_MAINMENUSCENE_HPP

#include <memory>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/UIElements/MenuScene.hpp>
#include <vvipers/UIElements/MenuButton.hpp>

namespace VVipers {

class MainMenuScene : public MenuScene {
  public:
    MainMenuScene(Game& game);
    void onMenuItemActivation(MenuItem* menuItem) override;
    scene_ptr makeTransition() override;

  private:
    std::unique_ptr<MenuButton> m_playButton;
    std::unique_ptr<MenuButton> m_optionsButton;
    std::unique_ptr<MenuButton> m_quitButton;
    std::unique_ptr<Scene> m_transitionTo;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_MAINMENUSCENE_HPP
