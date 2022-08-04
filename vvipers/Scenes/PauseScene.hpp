#ifndef VVIPERS_SCENES_PAUSESCENE_HPP
#define VVIPERS_SCENES_PAUSESCENE_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include <vvipers/Scenes/UIElements/MenuScene.hpp>
#include <vvipers/Scenes/UIElements/MenuButton.hpp>

namespace VVipers {

class Game;

class PauseScene : public MenuScene {
  public:
    PauseScene(Game& game);
    void drawBackground(sf::RenderTarget& target,
                        sf::RenderStates states) const override;
    void onMenuItemActivation(MenuItem* menuItem) override;

  private:
    sf::Text m_pauseText;
    std::unique_ptr<MenuButton> m_continueButton;
    std::unique_ptr<MenuButton> m_quitButton;
    sf::RectangleShape m_background;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_PAUSESCENE_HPP
