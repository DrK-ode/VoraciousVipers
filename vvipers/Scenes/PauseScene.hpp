#ifndef VVIPERS_SCENES_PAUSESCENE_HPP
#define VVIPERS_SCENES_PAUSESCENE_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class PauseScene : public Scene {
  public:
    PauseScene(Game& game);
    void draw() override;
    scene_ptr makeTransition() override;
    void processEvents() override;
    void update(Time elapsedTime) override{};

  private:
    Game& m_game;
    sf::Text m_pauseText;
    sf::Text m_quitText;
    sf::RectangleShape m_background;
};

}  // namespace VVipers

#endif // VVIPERS_SCENES_PAUSESCENE_HPP
