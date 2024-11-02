#ifndef VVIPERS_UIELEMENTS_PLAYERPANEL_HPP
#define VVIPERS_UIELEMENTS_PLAYERPANEL_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/GameElements/Observer.hpp>
#include <vvipers/GameElements/Player.hpp>
#include <vvipers/UIElements/ProgressBar.hpp>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

class FontProvider;

class PlayerPanel : public sf::Drawable, public Observer {
  public:
    PlayerPanel(sf::View view, const Player* player,
                const FontProvider& fontProvider);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onNotify(const GameEvent* event) override;
    const Player* getPlayer() const { return m_player; }
    Vec2 getScoreTarget() const;
    sf::View getView() const { return m_view; }
    void setView(sf::View view) { m_view = view; }

  private:
    void updateNameString();
    void updateScoreString();
    void updateScoreLimits();
    void addScore(score_t score);

    sf::View m_view;
    const Player* m_player;
    score_t m_score;
    sf::Text m_nameText;
    const sf::Font* m_font;
    ProgressBar m_boostBar;
    ProgressBar m_scoreBar;
};

}  // namespace VVipers

#endif  // VVIPERS_UIELEMENTS_PLAYERPANEL_HPP
