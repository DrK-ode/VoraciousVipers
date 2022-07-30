#include <SFML/Graphics/RenderTarget.hpp>
#include <sstream>
#include <vvipers/ProgressBar.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

ProgressBar::ProgressBar()
    : m_vertical(false),
      m_invert(false),
      m_progressLow(0.),
      m_progressHigh(1.),
      m_showText(false),
      m_textStyle(ProgressTextStyle::Percent) {
    setSize(m_size);
    setBorderWidth(1);
    setProgress(m_progressLow);
    m_mainRect.setFillColor(sf::Color::Transparent);
    m_mainRect.setOutlineColor(sf::Color::Blue);
    m_barRect.setFillColor(sf::Color::Red);
    m_barRect.setOutlineColor(sf::Color::Transparent);
}

void ProgressBar::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_mainRect, states);
    target.draw(m_barRect, states);
    target.draw(m_text, states);
}

void ProgressBar::setPosition(Vec2 position) {
    m_position = position;
    m_mainRect.setPosition(position);
    updateBar();
}

void ProgressBar::setBorderWidth(double width) {
    m_mainRect.setOutlineThickness(width);
    m_barRect.setOutlineThickness(width);
}

void ProgressBar::setProgressLimits(double low, double high) {
    m_progressLow = low;
    m_progressHigh = high;
}

void ProgressBar::setProgress(double progress) {
    m_progress = progress;
    updateBar();
};

void ProgressBar::setSize(Vec2 size) {
    m_size = size;
    m_mainRect.setSize(size);
    updateBar();
}

void ProgressBar::setTextProperties(const sf::Font* font, double characterSize,
                                    sf::Color color, ProgressTextStyle style) {
    m_text.setFont(*font);
    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(color);
    m_textStyle = style;
}

void ProgressBar::setVertical(bool vertical) {
    m_vertical = vertical;
    updateBar();
}

void ProgressBar::setInvertedBar(bool invert) {
    m_invert = invert;
    updateBar();
}

void ProgressBar::updateBar() {
    double relativeProgress =
        std::max(0., std::min(1., (m_progress - m_progressLow) /
                                      (m_progressHigh - m_progressLow)));
    double barStart = (m_invert xor m_vertical) ? 1. - relativeProgress : 0.;

    if (m_vertical) {
        barStart *= m_size.y;
        m_barRect.setSize(sf::Vector2f(m_size.x, m_size.y * relativeProgress));
        m_barRect.setPosition(m_position.x, m_position.y + barStart);
    } else {
        barStart *= m_size.x;
        m_barRect.setSize(sf::Vector2f(m_size.x * relativeProgress, m_size.y));
        m_barRect.setPosition(m_position.x + barStart, m_position.y);
    }
    if (m_showText) {
        std::stringstream ss;
        switch (m_textStyle) {
            case ProgressTextStyle::IntegerRatio: {
                ss << int(m_progress) << " / " << int(m_progressHigh);
                break;
            }
            case ProgressTextStyle::Percent: {
                ss << int(100 * (m_progress - m_progressLow) /
                          (m_progressHigh - m_progressLow))
                   << '%';
                break;
            }
        }
        m_text.setString(ss.str());
        auto lb = m_text.getLocalBounds();
        m_text.setOrigin(0.5 * (lb.left + lb.width),
                         0.5 * (2*lb.top + lb.height));
        m_text.setPosition(m_position + 0.5 * m_size);
    }
}

}  // namespace VVipers
