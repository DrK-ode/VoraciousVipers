#ifndef VVIPERS_PROGRESSBAR_HPP
#define VVIPERS_PROGRESSBAR_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class ProgressBar : public sf::Drawable {
  public:
    ProgressBar();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setBorderColor(sf::Color color) { m_mainRect.setOutlineColor(color); }
    void setBackgroundColor(sf::Color color) { m_mainRect.setFillColor(color); }
    void setBarColor(sf::Color color) { m_barRect.setFillColor(color); }
    void setBorderWidth(double width);
    void setSize(Vec2 size);
    void setPosition(Vec2 pos);
    void setProgress(double progress);
    void setVertical(bool vertical);
    void setInvertedBar(bool invert);

  private:
    void updateBar();

    bool m_vertical;
    bool m_invert;
    Vec2 m_size;
    Vec2 m_position;
    double m_progress;
    sf::RectangleShape m_mainRect;
    sf::RectangleShape m_barRect;
};

}  // namespace VVipers

#endif  // VVIPERS_PROGRESSBAR_HPP
