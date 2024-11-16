#include "vvipers/Utilities/TriangleStripArray.hpp"

#include <stdexcept>
#include <vector>

#include "vvipers/Utilities/Shape.hpp"

namespace VVipers {

void TriangleStripArray::draw(sf::RenderTarget& target,
                              sf::RenderStates states) const {
    states.texture = texture;
    target.draw(&vertices[0], vertices.size(), sf::PrimitiveType::TriangleStrip,
                states);
}

std::vector<Polygon> TriangleStripArray::create_polygons(
    size_t number_of_polygons) const {
    if (number_of_polygons > vertices.size() - 2)
        throw std::runtime_error("Requesting too many polygons.");
    size_t vertices_per_polygon =
        (vertices.size() - 2) / number_of_polygons + 2;

    std::vector<Polygon> polygons;
    for (size_t poly_index = 0; poly_index < number_of_polygons; ++poly_index) {
        size_t begin_index = poly_index * (vertices_per_polygon - 2);
        size_t end_index = begin_index + vertices_per_polygon;
        // The last polygon might have more vertices
        if (poly_index == number_of_polygons - 1) {
            end_index = vertices.size();
        }
        std::vector<Vec2> corners;
        size_t number_of_corners = end_index - begin_index;
        corners.resize(number_of_corners);
        for (size_t vertex_index = 0; vertex_index < number_of_corners;
             ++vertex_index) {
            size_t corner_index = vertex_index % 2 == 0
                                      ? number_of_corners - 1 - vertex_index / 2
                                      : vertex_index / 2;
            corners[corner_index] =
                vertices[begin_index + vertex_index].position;
        }
        polygons.emplace_back(std::move(corners));
    }
    return polygons;
}
}  // namespace VVipers