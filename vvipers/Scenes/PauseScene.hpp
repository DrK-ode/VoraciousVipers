#ifndef VVIPERS_SCENES_PAUSESCENE_HPP
#define VVIPERS_SCENES_PAUSESCENE_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/Utilities/Time.hpp>

namespace VVipers {

class Game;

class PauseScene : public Scene {
  public:
    PauseScene(const Game& game);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    scene_ptr makeTransition() override;
    void processEvent(const sf::Event& event) override;
    void update(Time elapsedTime) override{};

  private:
    sf::Text m_pauseText;
    sf::Text m_quitText;
    sf::RectangleShape m_background;
};

}  // namespace VVipers

#endif // VVIPERS_SCENES_PAUSESCENE_HPP
