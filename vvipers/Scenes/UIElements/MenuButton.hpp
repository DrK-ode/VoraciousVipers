#ifndef VVIPERS_SCENES_UIELEMENTS_MENUBUTTON_HPP
#define VVIPERS_SCENES_UIELEMENTS_MENUBUTTON_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/Scenes/UIElements/MenuItem.hpp>

namespace VVipers {

class MenuButton : public MenuItem {
  public:
    MenuButton();
    void onGeometryChange() override;
    void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override;
    void update(Time elapsedTime);
    void setFont(const sf::Font& font);
    void setLabel(const std::string& label);
    void setBackgroundColor(sf::Color color) {m_box.setFillColor(color);}
    void setBorderColor(sf::Color color) {m_box.setOutlineColor(color);}

  private:
    sf::RectangleShape m_box;
    sf::Text m_text;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_UIELEMENTS_MENUBUTTON_HPP
