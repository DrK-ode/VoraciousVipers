#include <vvipers/PlayerPanel.hpp>
#include <vvipers/config.hpp>

namespace VVipers {

PlayerPanel::PlayerPanel(Vec2 size, const Player* player)
    : m_size(size), m_player(player) {
    // Load font
    m_font.loadFromFile(FONT_FILE_PATH);
    // Set font
    m_nameText.setFont(m_font);
    m_scoreText.setFont(m_font);
    // Set color
    m_nameText.setFillColor(sf::Color::Red);
    m_scoreText.setFillColor(sf::Color::Yellow);
    // Set size
    const int characterSize = size.y / 3;  // px
    m_nameText.setCharacterSize(characterSize);
    m_scoreText.setCharacterSize(characterSize);
    // Set position
    // Set the strings
    m_nameText.setString("A");   // Dummy text to get default bounds
    m_scoreText.setString("A");  // Dummy text to get default bounds
    m_nameText.setOrigin(0, m_nameText.getLocalBounds().height / 2);
    m_scoreText.setOrigin(0, m_scoreText.getLocalBounds().height / 2);
    m_nameText.setPosition(0, size.y / 3);
    m_scoreText.setPosition(0, size.y * 2 / 3.);
    // Set the strings
    updateName();
    updateScore();
}

void PlayerPanel::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_nameText, states);
    target.draw(m_scoreText, states);
}

void PlayerPanel::onNotify(const GameEvent* event) {
    if (event->type() == GameEvent::EventType::Scoring) {
        if (static_cast<const ScoringEvent*>(event)->player == m_player)
            updateScore();
    }
}

void PlayerPanel::updateName(){
    m_nameText.setString(m_player->name());
}

void PlayerPanel::updateScore() {
    m_scoreText.setString(std::to_string(m_player->score()));
}

}  // namespace VVipers
