#ifndef VIPER_HPP
#define VIPER_HPP

#include <SFML/Graphics.hpp>
#include <vector>

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
    void setHeadVertices(TrackPoint* tp_front, TrackPoint* tp_back,
                         sf::Vertex array[]);
    void setBodyVertices(TrackPoint* tp_front, TrackPoint* tp_back,
                         sf::Vertex array[]);
    void setTailVertices(TrackPoint* tp_front, TrackPoint* tp_back,
                         sf::Vertex array[]);
    static const float s_segmentLength;
    static const float s_segmentWidth;
    static const float s_nominalSpeed;  // px/s
    static const uint32_t s_nPtsPerSegment;
    static const uint32_t s_nVerticesHead;
    static const uint32_t s_nVerticesBody;
    static const uint32_t s_nVerticesTail;
    float m_angle;  // degrees, clockwise since y-axis is downwards
    float m_speed;  // px/s
    float m_acc;    // px/s²
    int32_t m_growth;
    sf::Color m_color;
    TrackPoint* m_head;
    TrackPoint* m_tail;
    Track m_track;
    std::vector<sf::Vertex> m_vertices;
    sf::Texture m_texture;
};

#endif
