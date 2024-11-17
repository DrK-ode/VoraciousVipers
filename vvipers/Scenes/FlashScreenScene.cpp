#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Scenes/FlashScreenScene.hpp>
#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

namespace VVipers {

FlashScreenScene::FlashScreenScene(Game& game, Time duration)
    : Scene(game), _time_left(duration) {
    Vec2 size = game.window().getSize();
    _text.setFont(*game.font_service().default_font());
    _text.setString("Voracious Vipers");
    _text.setCharacterSize(0.1 * size.y);
    _text.setPosition(size / 2);
    auto lb = _text.getLocalBounds();
    _text.setOrigin(Vec2(lb.left + lb.width, lb.top + lb.height) / 2);
    _text.setFillColor(game.color_service().get_color(0));
    _text.setOutlineColor(game.color_service().get_color(1));
    _text.setOutlineThickness(std::max(1., 0.025 * _text.getCharacterSize()));
}

void FlashScreenScene::draw(sf::RenderTarget& target,
                            sf::RenderStates states) const {
    target.draw(_text, states);
}

void FlashScreenScene::update(Time elapsedTime) {
    _time_left -= elapsedTime;
    if (_time_left <= time_from_seconds(0)) {
        set_transition_state(TransitionState::Default);
    }
}

void FlashScreenScene::process_event(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed: {
            set_transition_state(TransitionState::Quit);
            break;
        }
        case sf::Event::KeyPressed:
        case sf::Event::MouseButtonPressed: {
            set_transition_state(TransitionState::Default);
            break;
        }
        default:
            break;
    }
}

}  // namespace VVipers
