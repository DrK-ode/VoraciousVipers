#pragma once

#include <SFML/Graphics/View.hpp>
#include <vector>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Engine/Scene.hpp>
#include <vvipers/UIElements/MenuItem.hpp>
#include <vvipers/Utilities/Time.hpp>
#include "vvipers/GameElements/Observer.hpp"

namespace VVipers {

class Game;

class MenuScene : public Scene, public Observer {
  public:
    enum LayoutOrientation { Horizontal, Vertical };

    MenuScene(Game& game);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void distribute_menu_items();
    void set_colors( sf::Color fill, sf::Color border, sf::Color text);
    void process_event(const sf::Event& event) override;
    void update(Time elapsedTime) override;
    void update_menu_items(Time elapsedTime);
    sf::View menu_view() const { return _menu_view; }
    void set_menu_view(sf::View view) { _menu_view = view; }
    void add_item(MenuItem* menuItem);
    void delete_item(size_t index);
    const MenuItem* item(int index) const { return _menu_items[index]; }
    const MenuItem* selected() const;
    void set_selected_index(size_t index);
    size_t number_of_menu_items() const { return _menu_items.size(); }
    LayoutOrientation layout_orientation() const { return _layout; }
    void set_layout_orientation(LayoutOrientation lo) { _layout = lo; }
    virtual void on_menu_item_activation(MenuItem* menuItem){};
    virtual void on_return() { set_transition_state(TransitionState::Return); }
    virtual void on_activation() override;
    void menu_up();
    void menu_down();
    void activate_selected();
    virtual void handle_key_pressed(const sf::Event& event);
    virtual void handle_mouse_moved(const sf::Event& event);
    virtual void handle_mouse_button_pressed(const sf::Event& event);
    virtual void handle_mouse_scroll(const sf::Event& event);
    virtual void handle_text_entered(const sf::Event& event);

  protected:
    virtual void draw_background(sf::RenderTarget& target,
                        sf::RenderStates states) const {};

  private:
    MenuItem* menu_item_at_coordinates(Vec2 coords) const;
    void swap_selected(size_t oldSelected, size_t newSelected);

    sf::View _menu_view;
    std::vector<MenuItem*> _menu_items;
    size_t _selected_index;
    LayoutOrientation _layout;
};

}  // namespace VVipers
