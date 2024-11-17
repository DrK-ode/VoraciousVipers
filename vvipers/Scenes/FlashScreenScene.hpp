#pragma once

#include <SFML/Graphics/Text.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class FlashScreenScene : public Scene {
  public:
    FlashScreenScene(Game& game, Time duration = time_from_seconds(2));
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void process_event(const sf::Event& event) override;
    void update(Time elapsedTime) override;

  private:
    Time _time_left;
    sf::Text _text;
};

}  // namespace VVipers
