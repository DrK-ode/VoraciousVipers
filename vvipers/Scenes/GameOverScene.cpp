#include "vvipers/Scenes/GameOverScene.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>

#include "vvipers/Engine/GameResources.hpp"
#include "vvipers/Engine/Scene.hpp"
#include "vvipers/GameElements/GameEvent.hpp"
#include "vvipers/Scenes/FlashScreenScene.hpp"
#include "vvipers/Utilities/Vec2.hpp"
#include "vvipers/Utilities/debug.hpp"

namespace VVipers {

GameOverScene::GameOverScene(GameResources& game,
                             std::vector<const Player*> players)
    : Scene(game), _players(players) {
    Vec2 size = game.window_manager().window_size();
    _game_over_text.setFont(*game.font_service().default_font());
    _game_over_text.setString("Game Over");
    _game_over_text.setCharacterSize(0.1 * size.y);
    _game_over_text.setPosition(size / 2);
    auto lbp = _game_over_text.getLocalBounds();
    _game_over_text.setOrigin(Vec2(lbp.left + lbp.width, lbp.top + lbp.height) /
                              2);
    _game_over_text.setFillColor(game.color_service().get_color(0));
    _game_over_text.setOutlineColor(game.color_service().get_color(1));
    _game_over_text.setOutlineThickness(0.005 * size.y);

    _score_text.setFont(*game.font_service().default_font());
    _score_text.setString(score_string(players));
    _score_text.setCharacterSize(0.5 * _game_over_text.getCharacterSize());
    _score_text.setPosition(_game_over_text.getPosition() +
                            Vec2(0, 2 * _game_over_text.getCharacterSize()));
    auto lbq = _score_text.getLocalBounds();
    _score_text.setOrigin(Vec2(lbq.left + lbq.width, lbq.top + lbq.height) / 2);
    _score_text.setFillColor(game.color_service().get_color(0));
    _score_text.setOutlineColor(game.color_service().get_color(1));
    _score_text.setOutlineThickness(0.5 *
                                    _game_over_text.getOutlineThickness());

    _background.setPosition(0, 0);
    _background.setSize(size);
    _background.setFillColor(sf::Color(0, 0, 0, 0x80));

    set_draw_state(DrawState::Transparent);
}

std::string GameOverScene::score_string(std::vector<const Player*>& players) {
    std::sort(players.begin(), players.end(),
              [](const auto& lhs, const auto& rhs) {
                  return lhs->score() > rhs->score();
              });
    std::stringstream ss;
    for (auto p = players.cbegin(); p != players.cend(); ++p) {
        if (p != players.cbegin())
            ss << '\n';
        ss << (*p)->name() << ": " << (*p)->score() << " (Viper level "
           << Player::calculate_level((*p)->score()) << ")";
    }
    return ss.str();
}

void GameOverScene::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
    target.draw(_background, states);
    target.draw(_game_over_text, states);
    target.draw(_score_text, states);
}

void GameOverScene::on_notify(const GameEvent& event) {
    switch (event.type()) {
        case GameEvent::EventType::Keyboard: {
            const KeyboardEvent& keyboard_event =
                dynamic_cast<const KeyboardEvent&>(event);
            if ((keyboard_event.scancode == sf::Keyboard::Scan::Enter) ||
                (keyboard_event.scancode == sf::Keyboard::Scan::Escape))
                notify(SceneEvent(SceneEvent::SceneEventType::Default));
            break;
        }
        case GameEvent::EventType::Mouse: {
            const MouseEvent& mouse_event =
                dynamic_cast<const MouseEvent&>(event);
            if (mouse_event.mouse_event_type ==
                MouseEvent::MouseEventType::ButtonPressed) {
                notify(SceneEvent(SceneEvent::SceneEventType::Default));
            }
            break;
        }
        default:
            break;
    }
}

}  // namespace VVipers