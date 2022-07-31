#ifndef VVIPERS_PROGRESSBAR_HPP
#define VVIPERS_PROGRESSBAR_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vvipers/Utilities/Vec2.hpp>

namespace VVipers {

class ProgressBar : public sf::Drawable {
  public:
    enum class ProgressTextStyle { Percent, IntegerRatio };

    ProgressBar();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Vec2 getPosition() const { return m_position; }
    sf::FloatRect getLocalBounds() const { return m_mainRect.getLocalBounds(); }
    double getProgress() const {return m_progress;}
    std::pair<double, double> getProgressLimits() const {
        return {m_progressLow, m_progressHigh};
    }

    void setBorderColor(sf::Color color) { m_mainRect.setOutlineColor(color); }
    void setBackgroundColor(sf::Color color) { m_mainRect.setFillColor(color); }
    void setBarColor(sf::Color color) { m_barRect.setFillColor(color); }
    void setBorderWidth(double width);
    void setProgressLimits(double low, double high);
    void setShowText(bool show) { m_showText = show; }
    void setSize(Vec2 size);
    void setTextProperties(const sf::Font* font, double sizeRatio,
                           sf::Color color, ProgressTextStyle style);
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
    double m_progressLow;
    double m_progressHigh;
    bool m_showText;
    sf::Text m_text;
    ProgressTextStyle m_textStyle;
    sf::RectangleShape m_mainRect;
    sf::RectangleShape m_barRect;
};

}  // namespace VVipers

#endif  // VVIPERS_PROGRESSBAR_HPP
