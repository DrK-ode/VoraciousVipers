#ifndef VVIPER_VIPER_HPP
#define VVIPER_VIPER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <vvipers/Track.hpp>
#include <vvipers/ViperVertices.hpp>

namespace VVipers {

class Viper : public sf::Drawable {
  public:
    Viper();
    ~Viper();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    float angle() const { return m_angle; }
    void setAngle(float a) { m_angle = mod180Deg(a); }
    void growth( const sf::Time& g );
    float length() const;
    void setup(const Vec2f& from, float angle, const sf::Time& length);
    float speed() const {return m_speed;}
    void tick(sf::Time elapsedTime);

  private:
    TrackPoint* createNextHeadTrackPoint(sf::Time elapsedTime);
    void cleanUpTrailingTrackPoints();
    void grow( sf::Time elapsedTime);
    static const float s_nominalSpeed;  // px/s
    float m_angle;  // degrees, clockwise since y-axis is downwards
    float m_speed;  // px/s
    float m_acc;    // px/s²
    sf::Time m_temporalLength;
    sf::Time m_growth;
    TrackPoint* m_head;
    Track m_track;
    ViperVertices m_vertices;
};

}  // namespace VVipers
#endif