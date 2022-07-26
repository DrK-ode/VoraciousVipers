#ifndef VVIPERS_PLAYERPANEL_HPP
#define VVIPERS_PLAYERPANEL_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/Observer.hpp>
#include <vvipers/Player.hpp>
#include <vvipers/Vec2.hpp>
#include <SFML/Graphics/Font.hpp>

namespace VVipers {

class PlayerPanel : public sf::Drawable, public Observer {
  public:
    PlayerPanel(Vec2 size, const Player* player);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onNotify(const GameEvent* event) override;
    const Player* player() const {return m_player;}

  private:
    void updateName();
    void updateScore();

    const Player* m_player;
    Vec2 m_size;
    sf::Text m_nameText;
    sf::Text m_scoreText;
    sf::Font m_font;
};

}  // namespace VVipers

#endif  // VVIPERS_PLAYERPANEL_HPP
