#ifndef VIPER_HPP
#define VIPER_HPP

#include <SFML/Graphics.hpp>

#include "Track.hpp"

class Viper : public sf::Drawable {
  public:
    Viper();
    ~Viper();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    float length() const;
    void setupStart(const Vec2f& from, float angle, uint32_t nSeg);
    void tick(sf::Time elapsedTime);
    void updateVertices();

  private:
    void createNextTrackPoint(sf::Time elapsedTime);
    void moveHead(int frames);
    void moveTail(int frames);
    void cleanUpTrailingTrackPoints();
    static const float s_segmentLength;
    static const float s_segmentWidth;
    static const float s_nominalSpeed;  // px/s
    static const float s_pointsPerSegment;
    static const uint32_t s_nVerticesHead;
    static const uint32_t s_nVerticesBody;
    static const uint32_t s_nVerticesTail;
    float m_angle;  // degrees, clockwise since y-axis is downwards
    float m_speed;  // px/s
    float m_acc;    // px/s²
    int32_t m_nSegments;
    int32_t m_growth;
    sf::Color m_color1;
    sf::Color m_color2;
    TrackPoint* m_head;
    TrackPoint* m_tail;
    Track m_track;
    sf::VertexArray m_vertices;
};

#endif
