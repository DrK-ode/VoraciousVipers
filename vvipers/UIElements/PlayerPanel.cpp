#include <vvipers/UIElements/PlayerPanel.hpp>
#include <vvipers/Engine/Providers.hpp>
#include <vvipers/GameElements/Viper.hpp>

namespace VVipers {

PlayerPanel::PlayerPanel(sf::View view, const Player* player,
                         const FontProvider& fontProvider)
    : m_view(view), m_player(player), m_score(player->score()) {
    m_font = fontProvider.default_font();
    // Set text properties
    m_nameText.setFont(*m_font);
    const int characterSize = 0.25 * m_view.getSize().y;  // px
    m_nameText.setCharacterSize(characterSize);
    // Set the name string and the position (dependent on string size)
    updateNameString();
    // Set positions
    Vec2 spacing = Vec2(0.025, 0.1) * Vec2(m_view.getSize());
    Vec2 boostBarPos = spacing;
    Vec2 boostBarSize = Vec2(0.05, 0.8) * Vec2(m_view.getSize());
    Vec2 scoreBarSize =
        Vec2(m_view.getSize().x - 3 * spacing.x - boostBarSize.x, 0.25 * m_view.getSize().y);
    Vec2 namePosition = boostBarPos + Vec2(spacing.x + boostBarSize.x,
                                           m_nameText.getLocalBounds().top);
    Vec2 scoreBarPos =
        Vec2(namePosition.x, m_view.getSize().y - 2 * spacing.y - scoreBarSize.y);
    m_boostBar.setPosition(spacing);
    m_nameText.setPosition(namePosition);
    m_scoreBar.setPosition(scoreBarPos);
    // Setup boost bar
    m_boostBar.setSize(boostBarSize);
    m_boostBar.setBorderWidth(2);
    m_boostBar.setBarColor(player->secondary_color());
    m_boostBar.setBorderColor(player->primary_color());
    m_boostBar.setVertical(true);
    m_boostBar.setProgress( time_as_seconds(player->viper()->boost_charge()));
    // Setup score bar
    m_scoreBar.setSize(scoreBarSize);
    m_scoreBar.setBorderWidth(2);
    m_scoreBar.setBarColor(player->secondary_color());
    m_scoreBar.setBorderColor(player->primary_color());
    m_scoreBar.setTextProperties(m_font, 0.8 * characterSize,
                                 player->primary_color(),
                                 ProgressBar::ProgressTextStyle::IntegerRatio);
    m_scoreBar.setShowText(true);
    updateScoreString();
}

void PlayerPanel::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_nameText, states);
    target.draw(m_scoreBar, states);
    target.draw(m_boostBar, states);
}

void PlayerPanel::on_notify(const GameEvent* event) {
    if (event->type() == GameEvent::EventType::Scoring) {
        auto scoringEvent = static_cast<const ScoringEvent*>(event);
        // We trust that nobody sends another player's score
        addScore(scoringEvent->score);
    } else if (event->type() == GameEvent::EventType::ObjectModified) {
        const ObjectModifiedEvent* boostEvent =
            static_cast<const ObjectModifiedEvent*>(event);
        if (typeid(*boostEvent->object_pointer) == typeid(Viper)) {
            const Viper* viper =
                static_cast<const Viper*>(boostEvent->object_pointer);
            m_boostBar.setProgress(viper->boost_charge() / viper->boost_max());
        } else if (typeid(*boostEvent->object_pointer) == typeid(Player)) {
            updateNameString();
        }
    }
}

void PlayerPanel::updateNameString() {
    m_nameText.setString(m_player->name());
    m_nameText.setFillColor(m_player->primary_color());
}

void PlayerPanel::updateScoreString() {
    updateScoreLimits();
    m_scoreBar.setProgress(m_score);
}

Vec2 PlayerPanel::getScoreTarget() const {
    auto lb = m_scoreBar.getLocalBounds();
    auto [low, high] = m_scoreBar.getProgressLimits();
    return m_scoreBar.getPosition() +
           Vec2(0.5, (m_scoreBar.getProgress() - low) / (high - low)) *
               Vec2(lb.left + lb.width, lb.top + lb.height);
}

void PlayerPanel::addScore(score_t score) {
    m_score += score;
    updateScoreString();
}

void PlayerPanel::updateScoreLimits() {
    m_scoreBar.setProgressLimits(
        0, Player::calculate_level_limit(Player::calculate_level(m_score) + 1));
}

}  // namespace VVipers
