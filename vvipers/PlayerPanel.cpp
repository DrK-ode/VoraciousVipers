#include <sstream>
#include <vvipers/PlayerPanel.hpp>
#include <vvipers/config.hpp>

namespace VVipers {

PlayerPanel::PlayerPanel(Vec2 size, const Player* player)
    : m_size(size), m_player(player), m_score(player->score()) {
    // Load and set font
    m_font.loadFromFile(FONT_FILE_PATH);
    m_nameText.setFont(m_font);
    m_scoreText.setFont(m_font);
    // Set text color
    m_nameText.setFillColor(sf::Color::Red);
    m_scoreText.setFillColor(sf::Color::Yellow);
    // Set text size
    const int characterSize = size.y / 3;  // px
    m_nameText.setCharacterSize(characterSize);
    m_scoreText.setCharacterSize(characterSize);
    // Set the strings and position (dependent on string size)
    updateNameString();
    updateScoreString();
    // The bounds are assuming the tallest possible character but need strings
    // in order to compute non-zero
    auto lbn = m_nameText.getLocalBounds();
    m_nameText.setOrigin(0, (lbn.height + lbn.top) / 2.);
    m_nameText.setPosition(40, m_size.y / 3.);
    auto lbs = m_scoreText.getLocalBounds();
    m_scoreText.setOrigin(0, (lbs.height + lbs.top) / 2);
    m_scoreText.setPosition(40, m_size.y * 2 / 3.);
    // Setup boost bar
    m_boostBar.setSize({20., 0.9 * size.y});
    m_boostBar.setBorderWidth(2);
    m_boostBar.setBorderColor(player->color());
    m_boostBar.setProgress(0.3);
    m_boostBar.setVertical(true);
    m_boostBar.setPosition({10, 0.05 * size.y});
}

void PlayerPanel::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_nameText, states);
    target.draw(m_scoreText, states);
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

void PlayerPanel::updateNameString() { m_nameText.setString(m_player->name()); }

void PlayerPanel::updateScoreString() {
    std::stringstream ss;
    ss << "Score: " << m_score;
    m_scoreText.setString(ss.str());
}

Vec2 PlayerPanel::getScoreTarget() const { return m_scoreText.getPosition(); }

void PlayerPanel::addScore(score_t score) {
    m_score += score;
    updateScoreString();
}

}  // namespace VVipers
