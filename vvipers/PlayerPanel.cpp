#include <sstream>
#include <vvipers/GameOptions.hpp>
#include <vvipers/PlayerPanel.hpp>

namespace VVipers {

PlayerPanel::PlayerPanel(Vec2 size, const Player* player)
    : m_size(size), m_player(player), m_score(player->score()) {
    // Load font
    std::stringstream ss;
    ss << GameOptions::getOptionString("General/resourceDirectoryPath")
       << GameOptions::getOptionString("General/fontFileName");
    m_font.loadFromFile(ss.str());
    // Set text properties
    m_nameText.setFont(m_font);
    const int characterSize = size.y / 4;  // px
    m_nameText.setCharacterSize(characterSize);
    // Set the name string and the position (dependent on string size)
    updateNameString();
    // Set positions
    m_boostBar.setPosition(Vec2(0.025, 0.1) * size);
    m_nameText.setPosition(Vec2(0.1, 1. / 8.) * m_size);
    m_scoreBar.setPosition(Vec2(0.1, 5. / 8.) * m_size);
    // Setup boost bar
    m_boostBar.setSize(Vec2(0.05, 0.8) * m_size);
    m_boostBar.setBorderWidth(2);
    m_boostBar.setBarColor(sf::Color::Red);
    m_boostBar.setBorderColor(player->color());
    m_boostBar.setVertical(true);
    // Setup score bar
    m_scoreBar.setSize(Vec2(0.6, 2. / 8.) * m_size);
    m_scoreBar.setBorderWidth(2);
    m_scoreBar.setBarColor(sf::Color::Yellow);
    m_scoreBar.setBorderColor(sf::Color::Yellow);
    m_scoreBar.setTextProperties(m_font, 0.8 * characterSize,
                                 sf::Color::Magenta,
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

void PlayerPanel::onNotify(const GameEvent* event) {
    if (event->type() == GameEvent::EventType::Scoring) {
        auto scoringEvent = static_cast<const ScoringEvent*>(event);
        // We trust that nobody sends another player's score
        addScore(scoringEvent->score);
    } else if (event->type() == GameEvent::EventType::Boost) {
        const BoostEvent* boostEvent = static_cast<const BoostEvent*>(event);
        m_boostBar.setProgress(boostEvent->chargeCurrent /
                               boostEvent->chargeMax);
    }
}

void PlayerPanel::updateNameString() {
    m_nameText.setString(m_player->name());
    m_nameText.setFillColor(m_player->color());
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
        0, Player::calculateLevelLimit(Player::calculateLevel(m_score) + 1));
}

}  // namespace VVipers
