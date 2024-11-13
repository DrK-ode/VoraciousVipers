#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <vvipers/GameElements/Walls.hpp>
#include <vvipers/Utilities/VVMath.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/Collisions/CollidingSegment.hpp"
#include "vvipers/Utilities/Vec2.hpp"

namespace VVipers {

Walls::Walls(Vec2 levelSize) : _level_size(levelSize) { constructLevel(); }

std::vector<std::shared_ptr<const CollidingSegment>> Walls::colliding_segments()
    const {
    std::vector<std::shared_ptr<const CollidingSegment>> vec;
    for (size_t i = 0; i < _polygons.size(); ++i) {
        std::vector<Vec2> coordinates;
        for( sf::Vertex vertex : _polygons[i]){
            coordinates.emplace_back(vertex.position);
        }
        vec.emplace_back(new CollidingPolygon(coordinates, this, i));
    }
    return vec;
}

void Walls::constructLevel() {
    const double width = _level_size.x;
    const double height = _level_size.y;
    const double edge = 5;
    // TODO: Textures
    auto color = sf::Color(0x80, 0x80, 0x80, 255);

    std::vector<sf::Vertex> top;
    top.emplace_back(sf::Vector2f(0, 0), color);
    top.emplace_back(sf::Vector2f(width, 0), color);
    top.emplace_back(sf::Vector2f(width, edge), color);
    top.emplace_back(sf::Vector2f(0, edge), color);
    std::vector<sf::Vertex> bottom;
    bottom.emplace_back(sf::Vector2f(0, height - edge), color);
    bottom.emplace_back(sf::Vector2f(width, height - edge), color);
    bottom.emplace_back(sf::Vector2f(width, height), color);
    bottom.emplace_back(sf::Vector2f(0, height), color);
    std::vector<sf::Vertex> left;
    left.emplace_back(sf::Vector2f(0, edge), color);
    left.emplace_back(sf::Vector2f(edge, edge), color);
    left.emplace_back(sf::Vector2f(edge, height - edge), color);
    left.emplace_back(sf::Vector2f(0, height - edge), color);
    std::vector<sf::Vertex> right;
    right.emplace_back(sf::Vector2f(width - edge, edge), color);
    right.emplace_back(sf::Vector2f(width, edge), color);
    right.emplace_back(sf::Vector2f(width, height - edge), color);
    right.emplace_back(sf::Vector2f(width - edge, height - edge), color);
    std::vector<sf::Vertex> middle;
    right.emplace_back(sf::Vector2f(width * 0.25, height * 0.375), color);
    right.emplace_back(sf::Vector2f(width * 0.75, height * 0.375), color);
    right.emplace_back(sf::Vector2f(width * 0.75, height * 0.625), color);
    right.emplace_back(sf::Vector2f(width * 0.25, height * 0.625), color);
}

void Walls::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto poly : _polygons)
        target.draw(&poly[0], poly.size(), sf::TriangleStrip, states);
}

}  // namespace VVipers