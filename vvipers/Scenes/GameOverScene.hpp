#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/GameElements/Player.hpp>
#include <vvipers/Utilities/Time.hpp>
#include "vvipers/GameElements/GameEvent.hpp"

namespace VVipers {

class GameResources;

class GameOverScene : public Scene {
  public:
    GameOverScene(GameResources& game, std::vector<const Player*> players);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void on_notify(const GameEvent&) override;
    void update(const Time&) override {}

  private:
    std::string score_string(std::vector<const Player*>& players);

    std::vector<const Player*> _players;
    sf::Text _game_over_text;
    sf::Text _score_text;
    sf::RectangleShape _background;
};

}  // namespace VVipers
