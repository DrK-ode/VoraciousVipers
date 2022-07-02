#ifndef VIPERVERTICES_HPP
#define VIPERVERTICES_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "Track.hpp"

class ViperVertices {
  public:
    ViperVertices();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(TrackPoint* tp_front, TrackPoint* tp_back, uint32_t nPtsPerSeg,
                float segmentWidth, float segmentLength);

  private:
    void prepareHead(TrackPoint* tp_front, uint32_t nPtsPerSeg,
                     float segmentWidth, float segmentLength);
    void prepareBody(TrackPoint* tp_front, uint32_t nPtsPerSeg,
                     uint32_t nBodySegments, float segmentWidth,
                     float segmentLength);
    void prepareTail(TrackPoint* tp_front, uint32_t nPtsPerSeg,
                     float segmentWidth, float segmentLength);
    std::vector<sf::Vertex> m_headVertices;
    std::vector<sf::Vertex> m_bodyVertices;
    std::vector<sf::Vertex> m_tailVertices;
    sf::Color m_color;
    sf::Texture m_headTexture;
    sf::Texture m_bodyTexture;
    sf::Texture m_tailTexture;
};

#endif