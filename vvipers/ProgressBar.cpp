#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/ProgressBar.hpp>

namespace VVipers {

ProgressBar::ProgressBar() : m_vertical(false), m_invert(false) {
    setSize(m_size);
    setBorderWidth(1);
    setProgress(0);
    m_mainRect.setFillColor(sf::Color::Transparent);
    m_mainRect.setOutlineColor(sf::Color::Blue);
    m_barRect.setFillColor(sf::Color::Red);
    m_barRect.setOutlineColor(sf::Color::Transparent);
}

void ProgressBar::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    target.draw(m_mainRect, states);
    target.draw(m_barRect, states);
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

void ProgressBar::setProgress(double progress) {
    m_progress = std::max(0., std::min(1., progress));
    updateBar();
};

void ProgressBar::setSize(Vec2 size) {
    m_size = size;
    m_mainRect.setSize(size);
    updateBar();
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
    double barStart = (m_invert xor m_vertical) ? 1. - m_progress : 0.;

    if (m_vertical) {
        barStart *= m_size.y;
        m_barRect.setSize(sf::Vector2f(m_size.x, m_size.y * m_progress));
        m_barRect.setPosition(m_position.x, m_position.y + barStart);
    } else {
        barStart *= m_size.x;
        m_barRect.setSize(sf::Vector2f(m_size.x * m_progress, m_size.y));
        m_barRect.setPosition(m_position.x + barStart, m_position.y);
    }
}

}  // namespace VVipers
