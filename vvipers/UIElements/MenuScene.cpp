#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vvipers/UIElements/MenuScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/GameElements/GameEvent.hpp"

namespace VVipers {

MenuScene::MenuScene(GameResources& game)
    : Scene(game),
      _menu_view(game.window_manager().default_view()),
      _selected_index(-1),
      _layout(Vertical) {}

void MenuScene::add_item(MenuItem* menu_item) {
    menu_item->add_observer(this, {GameEvent::EventType::ObjectModified});
    _menu_items.push_back(menu_item);
}

void MenuScene::delete_item(size_t index) {
    _menu_items.erase(_menu_items.begin() + index);
    // If the iterator points to the object erased or beyond, it is invalidated
    if (_selected_index >= index)
        --_selected_index;
}

void MenuScene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    draw_background(target, states);
    target.setView(_menu_view);
    for (auto& menuItem : _menu_items)
        target.draw(*menuItem, states);
}

void MenuScene::distribute_menu_items() {
    Vec2 size = _menu_view.getSize();
    Vec2 position;
    if (_layout == Horizontal) {
        size.x /= _menu_items.size();
        for (auto& mi : _menu_items) {
            mi->set_position(position);
            mi->set_size(size);
            position.x += size.x;
        }
    } else {
        size.y /= _menu_items.size();
        for (auto& mi : _menu_items) {
            mi->set_position(position);
            mi->set_size(size);
            position.y += size.y;
        }
    }
}

void MenuScene::set_colors(sf::Color fill, sf::Color border, sf::Color text) {
    for (auto menu_item : _menu_items) {
        menu_item->set_colors(fill, border, text);
    }
}

void MenuScene::handle_key_pressed(const KeyboardEvent& event) {
    if ((_layout == Vertical && event.scancode == sf::Keyboard::Scan::Up) or
        (_layout == Horizontal && event.scancode == sf::Keyboard::Scan::Left)) {
        menu_up();
    } else if ((_layout == Vertical &&
                event.scancode == sf::Keyboard::Scan::Down) or
               (_layout == Horizontal &&
                event.scancode == sf::Keyboard::Scan::Right)) {
        menu_down();
    } else if (event.scancode == sf::Keyboard::Scan::Enter) {
        activate_selected();
    } else if (event.scancode == sf::Keyboard::Scan::Escape) {
        on_return();
        notify(SceneEvent(SceneEvent::SceneEventType::Return));
    }
}

void MenuScene::handle_mouse_moved(const MouseEvent& event) {
    if (game_resources().window_manager().is_mouse_grabbed())
        return;
    auto old_selected_index = _selected_index;
    Vec2 local_coordinates =
        game_resources().window_manager().map_pixel_values_to_coordinates(
            event.position, _menu_view);
    auto pointed_at = menu_item_at_coordinates(local_coordinates);
    for (size_t i = 0; i < _menu_items.size(); ++i) {
        if (_menu_items[i] == pointed_at) {
            _selected_index = i;
            break;
        }
    }
    swap_selected(old_selected_index, _selected_index);
}

void MenuScene::handle_mouse_button_pressed(const MouseEvent& event) {
    if (game_resources().window_manager().is_mouse_grabbed())
        activate_selected();
    auto local_coordinates =
        game_resources().window_manager().map_pixel_values_to_coordinates(
            event.position, _menu_view);
    auto clickedMenuItem = menu_item_at_coordinates(local_coordinates);
    if (!clickedMenuItem)
        return;
    for (size_t i = 0; i < _menu_items.size(); ++i)
        if (_menu_items[i] == clickedMenuItem) {
            _selected_index = i;
            activate_selected();
            return;
        }
}

void MenuScene::handle_mouse_scroll(const MouseEvent& event) {
    if (event.scroll > 0)
        menu_up();
    else if (event.scroll < 0)
        menu_down();
}

void MenuScene::menu_up() {
    auto oldSelectedIndex = _selected_index;
    if (_selected_index == 0)
        _selected_index = _menu_items.size();
    --_selected_index;
    swap_selected(oldSelectedIndex, _selected_index);
}

void MenuScene::menu_down() {
    auto oldSelectedIndex = _selected_index;
    if (oldSelectedIndex >= _menu_items.size())
        _selected_index = 0;
    else
        ++_selected_index;
    if (_selected_index == _menu_items.size())
        _selected_index = 0;
    swap_selected(oldSelectedIndex, _selected_index);
}

void MenuScene::on_notify(const GameEvent& event) {
    if (run_state() != RunState::Running) {
        return;
    }
    switch (event.type()) {
        case GameEvent::EventType::Keyboard: {
            const KeyboardEvent& keyboard_event =
                dynamic_cast<const KeyboardEvent&>(event);
            switch (keyboard_event.keyboard_event_type) {
                case KeyboardEvent::KeyboardEventType::KeyPressed: {
                    handle_key_pressed(keyboard_event);
                    break;
                }
                case KeyboardEvent::KeyboardEventType::KeyReleased: {
                    break;
                }
            }
            break;
        }
        case GameEvent::EventType::Mouse: {
            const MouseEvent& mouse_event =
                dynamic_cast<const MouseEvent&>(event);
            switch (mouse_event.mouse_event_type) {
                case MouseEvent::MouseEventType::ButtonPressed: {
                    handle_mouse_button_pressed(mouse_event);
                    break;
                }
                case MouseEvent::MouseEventType::ButtonReleased: {
                    break;
                }
                case MouseEvent::MouseEventType::Move: {
                    handle_mouse_moved(mouse_event);
                    break;
                }
                case MouseEvent::MouseEventType::Scroll: {
                    handle_mouse_scroll(mouse_event);
                    break;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
    // Send it forward to, e.g., menu items
    notify(event);
}

MenuItem* MenuScene::menu_item_at_coordinates(Vec2 coords) const {
    for (auto menu_item : _menu_items)
        if (menu_item->contains(coords))
            return menu_item;
    return nullptr;
}

const MenuItem* MenuScene::selected() const {
    if (_selected_index < _menu_items.size())
        return _menu_items[_selected_index];
    return nullptr;
}

void MenuScene::set_selected_index(size_t index) {
    swap_selected(_selected_index, index);
    _selected_index = index;
}

void MenuScene::activate_selected() {
    if (_selected_index < _menu_items.size())
        on_menu_item_activation(_menu_items[_selected_index]);
}

void MenuScene::swap_selected(size_t old_selected_index,
                              size_t new_selected_index) {
    if (old_selected_index < _menu_items.size())
        _menu_items[old_selected_index]->set_selected(false);
    if (new_selected_index < _menu_items.size())
        _menu_items[new_selected_index]->set_selected(true);
}

/** Default is to update all the menu items **/
void MenuScene::update(const Time& elapsed_time) {
    update_menu_items(elapsed_time);
}

void MenuScene::update_menu_items(Time elapsed_time) {
    for (auto& menu_item : _menu_items)
        menu_item->update(elapsed_time);
}

void MenuScene::on_activation() { set_run_state(RunState::Running); }

}  // namespace VVipers
