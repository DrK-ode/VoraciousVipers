#pragma once

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
                const FontProvider& font_provider);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void on_notify(const GameEvent& event) override;
    const Player* player() const { return _player; }
    Vec2 score_target() const;
    sf::View view() const { return _view; }
    void set_view(sf::View view) { _view = view; }

  private:
    void update_name_string();
    void update_score_string();
    void update_score_limits();
    void add_score(score_t score);

    sf::View _view;
    const Player* _player;
    score_t _score;
    sf::Text _name_text;
    const sf::Font* _font;
    ProgressBar _boost_bar;
    ProgressBar _score_bar;
};

}  // namespace VVipers
