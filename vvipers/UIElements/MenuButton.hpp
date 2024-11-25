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
    void set_font(const sf::Font& font);
    void set_label(const std::string& label);
    void set_colors(sf::Color fill, sf::Color border, sf::Color text) override;

  private:
    sf::RectangleShape _box;
    sf::Text _text;
};

}  // namespace VVipers
