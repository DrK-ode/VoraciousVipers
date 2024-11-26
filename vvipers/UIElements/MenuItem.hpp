#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <vvipers/Engine/Game.hpp>
#include <vvipers/Utilities/Time.hpp>
#include "vvipers/GameElements/Observer.hpp"

namespace VVipers {

class MenuItem : public sf::Drawable, public Observable {
  public:
    MenuItem() : _selected(false){};
    bool contains(double x, double y) const {
        return local_bounds().contains(x, y);
    }
    bool contains(Vec2 point) const { return contains(point.x, point.y); }
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override{};
    virtual void set_colors( sf::Color fill, sf::Color border, sf::Color text) {};
    virtual void update(Time elapsedTime){};
    void set_selected(bool selected);
    bool is_selected() const { return _selected; }
    sf::FloatRect local_bounds() const;
    Vec2 size() const { return _size; }
    void set_size(Vec2 size);
    Vec2 position() const { return _position; }
    void set_position(Vec2 position);
    virtual void on_geometry_change(){};
    virtual void on_selection(){};
    virtual void on_event(const sf::Event& event){};

  private:
    Vec2 _size;
    Vec2 _position;
    bool _selected;
};

}  // namespace VVipers
