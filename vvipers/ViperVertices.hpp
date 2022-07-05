#ifndef VIPERVERTICES_HPP
#define VIPERVERTICES_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <vvipers/Track.hpp>

namespace VVipers {

class ViperVertices {
  public:
    ViperVertices();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update( const sf::Time& timeFront, const sf::Time& temporalLength, const Track& timeTrack);

  private:
    static const sf::Time s_headTemporalLength;
    static const sf::Time s_bodyTemporalLength;
    static const sf::Time s_tailTemporalLength; // Actually a minimum, can be larger
    void prepareHead( const sf::Time& timeFront, const sf::Time& temporalLength, const Track& timeTrack);
    void prepareBody( const sf::Time& timeFront, const sf::Time& temporalLength, const Track& timeTrack, uint32_t nSegments);
    void prepareTail( const sf::Time& timeFront, const sf::Time& temporalLength, const Track& timeTrack);
    
    // Atm, the same color will be applied to the whole viper
    sf::Color m_color;
    // Each of head, body and tail has different shape and texture
    std::vector<sf::Vertex> m_headVertices;
    std::vector<sf::Vertex> m_bodyVertices;
    std::vector<sf::Vertex> m_tailVertices;
    sf::Texture m_headTexture;
    sf::Texture m_bodyTexture;
    sf::Texture m_tailTexture;
};

}  // namespace VVipers
#endif