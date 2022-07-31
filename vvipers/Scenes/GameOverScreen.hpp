#ifndef VVIPERS_SCENES_GAMEOVERSCREEN_HPP
#define VVIPERS_SCENES_GAMEOVERSCREEN_HPP

#include <vector>
#include <memory>
#include <vvipers/Scenes/GameElements/Player.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/Scenes/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class GameOverScreen : public Scene {
  public:
    GameOverScreen(Game& game, std::vector< std::shared_ptr<const Player>> players);
    void draw() override;
    void processEvents() override;
    void update(Time elapsedTime) override;
    scene_ptr makeTransition() override;

  private:
    Game& m_game;
    std::vector<std::shared_ptr<const Player>> m_players;
    sf::Text m_gameOverText;
    sf::Text m_scoreText;
    sf::RectangleShape m_background;
};

}  // namespace VVipers

#endif // VVIPERS_SCENES_GAMEOVERSCREEN_HPP
