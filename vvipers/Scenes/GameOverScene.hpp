#ifndef VVIPERS_SCENES_GAMEOVERSCENE_HPP
#define VVIPERS_SCENES_GAMEOVERSCENE_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <vvipers/GameElements/Player.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class GameOverScene : public Scene {
  public:
    GameOverScene(Game& game,
                   std::vector<const Player*> players);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void processEvent(const sf::Event& event) override;
    void update(Time elapsedTime) override;

  private:
    std::string getScoreString(
        std::vector<const Player*>& players);

    std::vector<const Player*> m_players;
    sf::Text m_gameOverText;
    sf::Text m_scoreText;
    sf::RectangleShape m_background;
};

}  // namespace VVipers

#endif // VVIPERS_SCENES_GAMEOVERSCENE_HPP
