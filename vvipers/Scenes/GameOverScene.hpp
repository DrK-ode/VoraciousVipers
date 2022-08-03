#ifndef VVIPERS_SCENES_GAMEOVERSCENE_HPP
#define VVIPERS_SCENES_GAMEOVERSCENE_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <vector>
#include <vvipers/Scenes/GameElements/Player.hpp>
#include <vvipers/Scenes/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class GameOverScene : public Scene {
  public:
    GameOverScene(Game& game,
                   std::vector<std::shared_ptr<const Player>> players);
    void draw() override;
    void processEvents() override;
    void update(Time elapsedTime) override;

  private:
    std::string getScoreString(
        std::vector<std::shared_ptr<const Player>>& players);

    Game& m_game;
    std::vector<std::shared_ptr<const Player>> m_players;
    sf::Text m_gameOverText;
    sf::Text m_scoreText;
    sf::RectangleShape m_background;
};

}  // namespace VVipers

#endif // VVIPERS_SCENES_GAMEOVERSCENE_HPP
