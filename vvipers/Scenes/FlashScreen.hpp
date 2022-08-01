#ifndef VVIPERS_FLASHSCREEN_HPP
#define VVIPERS_FLASHSCREEN_HPP

#include <SFML/Graphics/Text.hpp>
#include <vvipers/Scenes/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class FlashScreen : public Scene {
  public:
    FlashScreen(Game& game, Time duration = seconds(2));
    void draw() override;
    void processEvents() override;
    void update(Time elapsedTime) override;
    std::shared_ptr<Scene> makeTransition() override;

  private:
    Game& m_game;
    Time m_timeLeft;
    sf::Text m_text;
};

}  // namespace VVipers

#endif  // VVIPERS_FLASHSCREEN_HPP
