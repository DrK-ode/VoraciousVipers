#ifndef VIPER_HPP
#define VIPER_HPP

#include <SFML/Graphics.hpp>
#include <deque>

#include "ViperSegment.hpp"

class Viper : public sf::Drawable {
  public:
    Viper();
    ~Viper();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    float length() const;
    void setupStart(const Vec2f& from, const Vec2f& to, uint32_t nSeg);
    void tick(sf::Time elapsedTime);

  private:
    ViperSegment* head() const { return m_segments.front(); }
    ViperSegment* tail() const { return m_segments.back(); }
    float m_angle;         // degrees, clockwise since y-axis is downwards
    float m_speed;         // px/s
    float m_nominalSpeed;  // px/s
    float m_acc;           // px/s²
    Track m_track;
    std::vector<ViperSegment*> m_segments;
};

#endif
