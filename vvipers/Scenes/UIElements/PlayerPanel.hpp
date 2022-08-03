#ifndef VVIPERS_PLAYERPANEL_HPP
#define VVIPERS_PLAYERPANEL_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/Scenes/GameElements/Observer.hpp>
#include <vvipers/Scenes/GameElements/Player.hpp>
#include <vvipers/Scenes/UIElements/ProgressBar.hpp>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

class FontProvider;

class PlayerPanel : public sf::Drawable, public Observer {
  public:
    PlayerPanel(Vec2 size, const Player* player, const FontProvider& fontProvider);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onNotify(const GameEvent* event) override;
    const Player* getPlayer() const { return m_player; }
    Vec2 getScoreTarget() const;

  private:
    void updateNameString();
    void updateScoreString();
    void updateScoreLimits();
    void addScore(score_t score);

    Vec2 m_size;
    const Player* m_player;
    score_t m_score;
    sf::Text m_nameText;
    const sf::Font* m_font;
    ProgressBar m_boostBar;
    ProgressBar m_scoreBar;
};

}  // namespace VVipers

#endif  // VVIPERS_PLAYERPANEL_HPP
