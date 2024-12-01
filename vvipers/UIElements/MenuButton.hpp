#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/UIElements/MenuItem.hpp>

namespace VVipers {

class MenuButton : public MenuItem {
  public:
    MenuButton();
    void on_geometry_change() override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void on_selection() override;
    void set_text(const sf::Font& font, sf::Color text_color) override;
    void set_label(const std::string& label);
    void set_colors(sf::Color fill, sf::Color border) override;
    void on_enable() override;

  private:
    void update_colors();

    sf::RectangleShape _box;
    sf::Text _text;
    sf::Color _fill_color;
    sf::Color _border_color;
    sf::Color _text_color;
};

}  // namespace VVipers
