#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "vvipers/Utilities/Vec2.hpp"

namespace VVipers {

class WindowManager {
  public:
    WindowManager(const sf::Vector2u& window_size);
    void center_mouse();
    void clear(const sf::Color& color) { _window.clear(color); }
    sf::View default_view() const {return _window.getDefaultView();}
    void display() { _window.display(); }
    void draw(const sf::Drawable& drawable) { _window.draw(drawable); }
    bool is_mouse_grabbed() const { return _is_mouse_grabbed; }
    sf::Vector2i map_coordinates_to_pixel_values(const Vec2& coordinates,
                                                 const sf::View& view) const {
        return _window.mapCoordsToPixel(sf::Vector2f(coordinates), view);
    }
    Vec2 map_pixel_values_to_coordinates(const sf::Vector2i& pixel_values,
                                         const sf::View& view) const {
        return _window.mapPixelToCoords(pixel_values, view);
    }
    sf::Vector2i mouse_position() const;
    bool poll_event(sf::Event& event) { return _window.pollEvent(event); }
    void set_grab_mouse(bool grabbed);
    sf::Vector2u window_size() const { return _window.getSize(); }

  private:
    bool _is_mouse_grabbed;
    sf::RenderWindow _window;
};

}  // namespace VVipers