#ifndef VVIPERS_SCENES_UIELEMENTS_MENUBUTTON_HPP
#define VVIPERS_SCENES_UIELEMENTS_MENUBUTTON_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/UIElements/MenuItem.hpp>

namespace VVipers {

class MenuButton : public MenuItem {
  public:
    MenuButton();
    void onGeometryChange() override;
    void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override;
    void onSelection() override;
    void setFont(const sf::Font& font);
    void setLabel(const std::string& label);
    void setColors( sf::Color fill, sf::Color border, sf::Color text) override;
    void setFontRatio(double ratio){m_fontRatio = ratio;}

  private:
    double m_fontRatio;
    sf::RectangleShape m_box;
    sf::Text m_text;
};

}  // namespace VVipers

#endif  // VVIPERS_SCENES_UIELEMENTS_MENUBUTTON_HPP
