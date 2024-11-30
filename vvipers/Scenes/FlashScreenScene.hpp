#pragma once

#include <SFML/Graphics/Text.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class GameResources;

class FlashScreenScene : public Scene {
  public:
    FlashScreenScene(GameResources&, Time duration = time_from_seconds(2));
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void on_notify(const GameEvent& event) override;

  private:
    void update(Time elapsedTime);
    Time _time_left;
    sf::Text _text;
};

}  // namespace VVipers
