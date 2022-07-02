#ifndef VIPERVERTICES_HPP
#define VIPERVERTICES_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Track.hpp"

class ViperVertices {
  public:
    ViperVertices();
    static float getSegmentLength() {return s_segmentLength; }
    static float getSegmentWidth() {return s_segmentWidth; }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(TrackPoint* tp_front, TrackPoint* tp_back, uint32_t nPtsPerSeg);

  private:
    static const float s_segmentLength;
    static const float s_segmentWidth;
    void prepareHead(TrackPoint* tp_front, uint32_t nPtsPerSeg);
    void prepareBody(TrackPoint* tp_front, uint32_t nPtsPerSeg,
                     uint32_t nBodySegments);
    void prepareTail(TrackPoint* tp_front, uint32_t nPtsPerSeg);
    std::vector<sf::Vertex> m_headVertices;
    std::vector<sf::Vertex> m_bodyVertices;
    std::vector<sf::Vertex> m_tailVertices;
    sf::Color m_color;
    sf::Texture m_headTexture;
    sf::Texture m_bodyTexture;
    sf::Texture m_tailTexture;
};

#endif