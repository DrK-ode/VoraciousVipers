#ifndef VIPER_HPP
#define VIPER_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Track.hpp"
#include "ViperVertices.hpp"

class Viper : public sf::Drawable {
  public:
    Viper();
    ~Viper();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    float length() const;
    void setupStart(const Vec2f& from, float angle, uint32_t nSeg);
    void tick(sf::Time elapsedTime);

  private:
    void createNextTrackPoint(sf::Time elapsedTime);
    void moveHead(int frames);
    void moveTail(int frames);
    void cleanUpTrailingTrackPoints();
    static const float s_segmentLength;
    static const float s_segmentWidth;
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

#endif
