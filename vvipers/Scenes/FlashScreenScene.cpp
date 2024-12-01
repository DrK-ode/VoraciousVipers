#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/GameResources.hpp>
#include <vvipers/Scenes/FlashScreenScene.hpp>
#include <vvipers/Scenes/MainMenuScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/GameElements/GameEvent.hpp"

namespace VVipers {

FlashScreenScene::FlashScreenScene(GameResources& game_resources, Time duration)
    : Scene(game_resources), _time_left(duration) {
    Vec2 size = game_resources.window_manager()->window_size();
    _text.setFont(*game_resources.font_service().default_font());
    _text.setString("Voracious Vipers");
    _text.setCharacterSize(0.1 * size.y);
    _text.setPosition(size / 2);
    auto lb = _text.getLocalBounds();
    _text.setOrigin(Vec2(lb.left + lb.width, lb.top + lb.height) / 2);
    _text.setFillColor(game_resources.color_service().get_color(0));
    _text.setOutlineColor(game_resources.color_service().get_color(1));
    _text.setOutlineThickness(std::max(1., 0.025 * _text.getCharacterSize()));
}

void FlashScreenScene::draw(sf::RenderTarget& target,
                            sf::RenderStates states) const {
    target.draw(_text, states);
}

void FlashScreenScene::update(Time elapsedTime) {
    _time_left -= elapsedTime;
    if (_time_left <= time_from_seconds(0)) {
        notify(SceneEvent(SceneEvent::SceneEventType::Default));
    }
}

void FlashScreenScene::on_notify(const GameEvent& event) {
    switch (event.type()) {
        case GameEvent::EventType::Keyboard: {
            const KeyboardEvent& keyboard_event =
                dynamic_cast<const KeyboardEvent&>(event);
            if (keyboard_event.keyboard_event_type ==
                KeyboardEvent::KeyboardEventType::KeyPressed) {
                notify(SceneEvent(SceneEvent::SceneEventType::Default));
            }
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
        case GameEvent::EventType::Update: {
            const UpdateEvent& update_event =
                dynamic_cast<const UpdateEvent&>(event);
            update(update_event.elapsed_time);
            break;
        }
        default:
            break;
    }
}

}  // namespace VVipers
