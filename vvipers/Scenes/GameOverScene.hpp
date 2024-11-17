#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/GameElements/Player.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class GameOverScene : public Scene {
  public:
    GameOverScene(Game& game, std::vector<const Player*> players);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void process_event(const sf::Event& event) override;
    void update(Time elapsedTime) override;

  private:
    std::string score_string(std::vector<const Player*>& players);

    std::vector<const Player*> _players;
    sf::Text _game_over_text;
    sf::Text _score_text;
    sf::RectangleShape _background;
};

}  // namespace VVipers
