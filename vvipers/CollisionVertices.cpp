#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/CollisionVertices.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/debug.hpp>
#include <vvipers/Vec2.hpp>
#include <vvipers/Bodypart.hpp>

namespace VVipers {

CollisionVertices::CollisionVertices()
    : m_texture(nullptr), m_vertices(sf::PrimitiveType::TriangleStrip) {}

CollisionVertices::~CollisionVertices(){
    clearBodyParts();
}

void CollisionVertices::clear() {
    m_vertices.clear();
    clearBodyParts();
}

void CollisionVertices::clearBodyParts() {
    for (auto part : m_bodyParts)
        delete part;
    m_bodyParts.clear();
}

void CollisionVertices::assignBodyParts(size_t beginIndex, size_t length,
                                    size_t nodesPerBodyPart,
                                    const std::string& label, int sharedNodes,
                                    bool active, bool symmetric) {
    if (m_vertices.getVertexCount() - beginIndex - length < 0)
        throw std::out_of_range("Out of bounds when assigning body parts.");
    auto index1 = beginIndex;
    auto index2 = index1 + nodesPerBodyPart;
    auto numberOfBodyParts =
        (length - sharedNodes) / (nodesPerBodyPart - sharedNodes);
    m_bodyParts.reserve(m_bodyParts.size() + numberOfBodyParts);
    for (int i = 0; i < numberOfBodyParts; ++i) {
        std::vector<Vec2> nodes;
        for (int j = index1; j < index2; ++j)
            nodes.push_back(m_vertices[j].position);
        m_bodyParts.push_back(new Bodypart(nodes, label, active, symmetric));
        index1 = index2 - sharedNodes;
        index2 = index1 + nodesPerBodyPart;
    }
}

void CollisionVertices::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
    states.texture = m_texture;
    target.draw(m_vertices, states);
}

}  // namespace VVipers