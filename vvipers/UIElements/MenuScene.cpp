#include <SFML/Window/Mouse.hpp>
#include <vvipers/UIElements/MenuScene.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>
#include "vvipers/GameElements/GameEvent.hpp"

namespace VVipers {

MenuScene::MenuScene(Game& game)
    : Scene(game), _menu_view(game.window().getDefaultView()), _selected_index(-1), _layout(Vertical) {}

void MenuScene::add_item(MenuItem* menuItem) {
    _menu_items.push_back(menuItem);
}

void MenuScene::delete_item(size_t index) {
    _menu_items.erase(_menu_items.begin() + index);
    // If the iterator points to the object erased or beyond, it is invalidated
    if (_selected_index >= index)
        --_selected_index;
}

void MenuScene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    draw_background(target,states);
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

void MenuScene::set_colors( sf::Color fill, sf::Color border, sf::Color text){
    for( auto menu_item : _menu_items ){
        menu_item->set_colors(fill,border,text);
    }
}

void MenuScene::handle_key_pressed(const sf::Event& event) {
    if ((_layout == Vertical && event.key.code == sf::Keyboard::Up) or
        (_layout == Horizontal && event.key.code == sf::Keyboard::Left)) {
        menu_up();
    } else if ((_layout == Vertical && event.key.code == sf::Keyboard::Down) or
               (_layout == Horizontal &&
                event.key.code == sf::Keyboard::Right)) {
        menu_down();
    } else if (event.key.code == sf::Keyboard::Return) {
        activate_selected();
    } else if (event.key.code == sf::Keyboard::Escape) {
        on_return();
    } else {
        if (_selected_index < _menu_items.size())
            _menu_items[_selected_index]->on_event(event);
    }
}

void MenuScene::handle_mouse_moved(const sf::Event& event) {
    if ( game().is_mouse_grabbed())
        return;
    auto old_selected_index = _selected_index;
    auto& window = game().window();
    Vec2 local_coordinates =
        window.mapPixelToCoords(sf::Mouse::getPosition(window), _menu_view);
    auto pointedAt = menu_item_at_coordinates(local_coordinates);
    for (size_t i = 0; i < _menu_items.size(); ++i) {
        if (_menu_items[i] == pointedAt) {
            _selected_index = i;
            _menu_items[i]->on_event(event);
            break;
        }
    }
    swap_selected(old_selected_index, _selected_index);
}

void MenuScene::handle_mouse_button_pressed(const sf::Event& event) {
    if ( game().is_mouse_grabbed())
        activate_selected();
    auto& window = game().window();
    Vec2 localCoord =
        window.mapPixelToCoords(sf::Mouse::getPosition(window), _menu_view);
    auto clickedMenuItem= menu_item_at_coordinates(localCoord);
    if( !clickedMenuItem ) return;
    for( size_t i = 0; i < _menu_items.size(); ++i )
        if( _menu_items[i] == clickedMenuItem ){
            _selected_index = i;
            _menu_items[i]->on_event(event);
            activate_selected();
            return;
        }
}

void MenuScene::handle_mouse_scroll(const sf::Event& event) {
    if (event.mouseWheelScroll.delta > 0)
        menu_up();
    else if (event.mouseWheelScroll.delta < 0)
        menu_down();
}

void MenuScene::handle_text_entered(const sf::Event& event) {
    if (_selected_index < _menu_items.size())
        _menu_items[_selected_index]->on_event(event);
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

void MenuScene::process_event(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed: {
            set_transition_state(TransitionState::Quit);
            break;
        }
        case sf::Event::TextEntered: {
            handle_text_entered(event);
            break;
        }
        case sf::Event::KeyPressed: {
            handle_key_pressed(event);
            break;
        }
        case sf::Event::MouseButtonPressed: {
            handle_mouse_button_pressed(event);
            break;
        }
        case sf::Event::MouseMoved: {
            handle_mouse_moved(event);
            break;
        }
        case sf::Event::MouseWheelScrolled: {
            handle_mouse_scroll(event);
            break;
        }
        default:
            break;
    }
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

void MenuScene::swap_selected(size_t old_selected_index, size_t new_selected_index) {
    if (old_selected_index < _menu_items.size())
        _menu_items[old_selected_index]->set_selected(false);
    if (new_selected_index < _menu_items.size())
        _menu_items[new_selected_index]->set_selected(true);
}

/** Default is to update all the menu items **/
void MenuScene::update(Time elapsed_time) { update_menu_items(elapsed_time); }

void MenuScene::update_menu_items(Time elapsed_time) {
    for (auto& menu_item : _menu_items)
        menu_item->update(elapsed_time);
}

void MenuScene::on_activation() {
    set_run_state(RunState::Running);
    set_transition_state(TransitionState::Continue);
}

}  // namespace VVipers
