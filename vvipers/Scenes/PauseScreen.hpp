#ifndef VVIPERS_SCENES_PAUSESCREEN_HPP
#define VVIPERS_SCENES_PAUSESCREEN_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/Scenes/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class PauseScreen : public Scene {
  public:
    PauseScreen(Game& game);
    void draw() override;
    void processEvents() override;
    void update(Time elapsedTime) override;
    scene_ptr makeTransition() override;

  private:
    Game& m_game;
    sf::Text m_text;
    sf::RectangleShape m_background;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_PAUSESCREEN_HPP
