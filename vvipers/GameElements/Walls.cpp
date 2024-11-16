#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>
#include <vvipers/GameElements/Walls.hpp>
#include <vvipers/Utilities/VVMath.hpp>
#include <vvipers/Utilities/debug.hpp>

#include "vvipers/Collisions/CollidingBody.hpp"
#include "vvipers/Utilities/Vec2.hpp"

namespace VVipers {

Walls::Walls(Vec2 levelSize) : CollidingBody("Walls"), _level_size(levelSize) {
    constructLevel();
}

std::shared_ptr<const Shape> Walls::body_part_shape(size_t index) const {
    if (index >= number_of_body_parts())
        throw std::runtime_error("Requested body part index is too large.");
    return _polygons[index];
}

void Walls::constructLevel() {
    const double width = _level_size.x;
    const double height = _level_size.y;
    const double edge = 5;

    std::vector<Vec2> top;
    top.emplace_back(0, 0);
    top.emplace_back(width, 0);
    top.emplace_back(width, edge);
    top.emplace_back(0, edge);
    std::vector<Vec2> bottom;
    bottom.emplace_back(0, height - edge);
    bottom.emplace_back(width, height - edge);
    bottom.emplace_back(width, height);
    bottom.emplace_back(0, height);
    std::vector<Vec2> left;
    left.emplace_back(0, edge);
    left.emplace_back(edge, edge);
    left.emplace_back(edge, height - edge);
    left.emplace_back(0, height - edge);
    std::vector<Vec2> right;
    right.emplace_back(width - edge, edge);
    right.emplace_back(width, edge);
    right.emplace_back(width, height - edge);
    right.emplace_back(width - edge, height - edge);
    std::vector<Vec2> middle;
    middle.emplace_back(width * 0.25, height * 0.375);
    middle.emplace_back(width * 0.75, height * 0.375);
    middle.emplace_back(width * 0.75, height * 0.625);
    middle.emplace_back(width * 0.25, height * 0.625);

    _polygons.emplace_back(std::make_shared<Polygon>(std::move(top)));
    _polygons.emplace_back(std::make_shared<Polygon>(std::move(left)));
    _polygons.emplace_back(std::make_shared<Polygon>(std::move(right)));
    _polygons.emplace_back(std::make_shared<Polygon>(std::move(bottom)));
    _polygons.emplace_back(std::make_shared<Polygon>(std::move(middle)));

    // TODO: Textures
    auto color = sf::Color(0x80, 0x80, 0x80, 255);
    for (auto& polygon : _polygons) {
        _vertex_vectors.emplace_back(polygon->triangle_strip(color));
    }
}

void Walls::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto vertices : _vertex_vectors)
        target.draw(&vertices[0], vertices.size(), sf::TriangleStrip, states);
}

}  // namespace VVipers