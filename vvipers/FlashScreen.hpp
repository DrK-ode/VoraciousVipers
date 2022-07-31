#ifndef VVIPERS_FLASHSCREEN_HPP
#define VVIPERS_FLASHSCREEN_HPP

#include <SFML/Graphics/Text.hpp>
#include <vvipers/Scene.hpp>
#include <vvipers/Time.hpp>

namespace VVipers {

class Game;

class FlashScreen : public Scene {
  public:
    FlashScreen(Game& game, Time duration);
    void draw() override;
    void processEvents() override;
    void update(Time elapsedTime) override;
    std::unique_ptr<Scene> getTransition() override;

  private:
    Game& m_game;
    Time m_timeLeft;
    sf::Text m_text;
};

}  // namespace VVipers

#endif  // VVIPERS_FLASHSCREEN_HPP
