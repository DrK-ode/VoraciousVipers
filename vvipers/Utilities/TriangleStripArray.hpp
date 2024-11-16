#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <vector>
#include "vvipers/Utilities/Shape.hpp"

namespace VVipers {
class TriangleStripArray : public sf::Drawable {
  public:
    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override;
    std::vector<Polygon> create_polygons(size_t) const;
    std::vector<sf::Vertex> vertices;
    const sf::Texture* texture;
};
}  // namespace VVipers