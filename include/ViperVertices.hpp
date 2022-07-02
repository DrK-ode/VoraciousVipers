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
    void setHeadVertices(TrackPoint* tp_front, TrackPoint* tp_back,
                         uint32_t nPtsPerSeg, float segmentWidth,
                         float segmentLength, sf::Vertex array[]);
    void setBodyVertices(TrackPoint* tp_front, TrackPoint* tp_back,
                         uint32_t nPtsPerSeg, float segmentWidth,
                         float segmentLength, sf::Vertex array[]);
    void setTailVertices(TrackPoint* tp_front, TrackPoint* tp_back,
                         uint32_t nPtsPerSeg, float segmentWidth,
                         float segmentLength, sf::Vertex array[]);
    static const size_t s_nHeadVertices;
    static const size_t s_nBodyVertices;
    static const size_t s_nTailVertices;
    std::vector<sf::Vertex> m_vertices;
    sf::Color m_color;
    sf::Texture m_headTexture;
    sf::Texture m_bodyTexture;
    sf::Texture m_tailTexture;
};

#endif