#ifndef VVIPERS_FLASHSCREEN_HPP
#define VVIPERS_FLASHSCREEN_HPP

#include <SFML/Graphics/Text.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class FlashScreenScene : public Scene {
  public:
    FlashScreenScene(const Game& game, Time duration = seconds(2));
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void processEvent(const sf::Event& event) override;
    void update(Time elapsedTime) override;

  private:
    Time m_timeLeft;
    sf::Text m_text;
};

}  // namespace VVipers

#endif  // VVIPERS_FLASHSCREEN_HPP
