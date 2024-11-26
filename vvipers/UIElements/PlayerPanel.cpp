#include <vvipers/Engine/Providers.hpp>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/UIElements/PlayerPanel.hpp>

#include "vvipers/GameElements/GameEvent.hpp"

namespace VVipers {

PlayerPanel::PlayerPanel(sf::View view, const Player* player,
                         const FontProvider& font_provider)
    : _view(view), _player(player), _score(player->score()) {
    _font = font_provider.default_font();
    // Set text properties
    _name_text.setFont(*_font);
    const int characterSize = 0.25 * _view.getSize().y;  // px
    _name_text.setCharacterSize(characterSize);
    // Set the name string and the position (dependent on string size)
    update_name_string();
    // Set positions
    Vec2 spacing = Vec2(0.025, 0.1) * Vec2(_view.getSize());
    Vec2 boost_bar_position = spacing;
    Vec2 boost_bar_size = Vec2(0.05, 0.8) * Vec2(_view.getSize());
    Vec2 score_bar_size =
        Vec2(_view.getSize().x - 3 * spacing.x - boost_bar_size.x,
             0.25 * _view.getSize().y);
    Vec2 name_position =
        boost_bar_position +
        Vec2(spacing.x + boost_bar_size.x, _name_text.getLocalBounds().top);
    Vec2 score_bar_position = Vec2(
        name_position.x, _view.getSize().y - 2 * spacing.y - score_bar_size.y);
    _boost_bar.set_position(spacing);
    _name_text.setPosition(name_position);
    _score_bar.set_position(score_bar_position);
    // Setup boost bar
    _boost_bar.set_size(boost_bar_size);
    _boost_bar.set_border_width(2);
    _boost_bar.set_bar_color(player->secondary_color());
    _boost_bar.set_border_color(player->primary_color());
    _boost_bar.set_vertical(true);
    _boost_bar.set_progress(time_as_seconds(player->viper()->boost_charge()));
    // Setup score bar
    _score_bar.set_size(score_bar_size);
    _score_bar.set_border_width(2);
    _score_bar.set_bar_color(player->secondary_color());
    _score_bar.set_border_color(player->primary_color());
    _score_bar.set_text_properties(_font, 0.8 * characterSize,
                                 player->primary_color(),
                                 ProgressBar::ProgressTextStyle::IntegerRatio);
    _score_bar.set_show_text(true);
    update_score_string();
}

void PlayerPanel::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(_name_text, states);
    target.draw(_score_bar, states);
    target.draw(_boost_bar, states);
}

void PlayerPanel::on_notify(const GameEvent& event) {
    if (event.type() == GameEvent::EventType::Scoring) {
        const ScoringEvent& scoringEvent =
            dynamic_cast<const ScoringEvent&>(event);
        // We trust that nobody sends another player's score
        add_score(scoringEvent.score);
    } else if (event.type() == GameEvent::EventType::ObjectModified) {
        const ObjectModifiedEvent& boostEvent =
            dynamic_cast<const ObjectModifiedEvent&>(event);
        if (typeid(*boostEvent.object_pointer) == typeid(Viper)) {
            const Viper* viper =
                static_cast<const Viper*>(boostEvent.object_pointer);
            _boost_bar.set_progress(viper->boost_charge() / viper->boost_max());
        } else if (typeid(*boostEvent.object_pointer) == typeid(Player)) {
            update_name_string();
        }
    }
}

void PlayerPanel::update_name_string() {
    _name_text.setString(_player->name());
    _name_text.setFillColor(_player->primary_color());
}

void PlayerPanel::update_score_string() {
    update_score_limits();
    _score_bar.set_progress(_score);
}

Vec2 PlayerPanel::score_target() const {
    auto bounds = _score_bar.local_bounds();
    auto [low, high] = _score_bar.progress_limits();
    return _score_bar.position() +
           Vec2(0.5, (_score_bar.progress() - low) / (high - low)) *
               Vec2(bounds.left + bounds.width, bounds.top + bounds.height);
}

void PlayerPanel::add_score(score_t score) {
    _score += score;
    update_score_string();
}

void PlayerPanel::update_score_limits() {
    _score_bar.set_progress_limits(
        0, Player::calculate_level_limit(Player::calculate_level(_score) + 1));
}

}  // namespace VVipers
