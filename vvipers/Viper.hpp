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
    float getAngle() const { return m_angle; }
    void setAngle(float a) { m_angle = a; }
    void growSegment(float growth);
    float length() const;
    void setup(const Vec2f& from, float angle, uint32_t nSeg);
    void tick(sf::Time elapsedTime);

  private:
    void createNextTrackPoint(sf::Time elapsedTime);
    void moveHead(int frames);
    void moveTail(int frames);
    void cleanUpTrailingTrackPoints();
    static const float s_nominalSpeed;  // px/s
    static const uint32_t s_nPtsPerSegment;
    float m_angle;  // degrees, clockwise since y-axis is downwards
    float m_speed;  // px/s
    float m_acc;    // px/s²
    int32_t m_growth;
    TrackPoint* m_head;
    TrackPoint* m_tail;
    Track m_track;
    ViperVertices m_vertices;
};

}  // namespace VVipers
#endif