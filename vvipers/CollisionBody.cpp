#include <SFML/Graphics/RenderTarget.hpp>
#include <vvipers/CollisionBody.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

void CollisionBody::assignBodyParts(size_t beginIndex, size_t length,
                                    std::string label, size_t nodesPerBodyPart,
                                    int sharedNodes, bool active,
                                    bool symmetric) {
    if (m_vertices.getVertexCount() - beginIndex - length < 0)
        throw std::out_of_range("Out of bounds when assigning body parts.");
    auto index1 = beginIndex;
    auto index2 = index1 + nodesPerBodyPart;
    auto numberOfBodyParts = (length - sharedNodes) / (nodesPerBodyPart - sharedNodes);
    m_bodyParts.reserve( m_bodyParts.size() + numberOfBodyParts);
    for( int i = 0; i < numberOfBodyParts; ++i ){
        std::vector<Vec2> nodes(nodesPerBodyPart);
        for( int j = index1; j < index2; ++j )
            nodes.push_back(m_vertices[j].position);
        m_bodyParts.emplace_back(nodes, label, active, symmetric);
        index1 = index2 - sharedNodes;
        index2 = index1 + nodesPerBodyPart;
    }
}

std::vector<std::pair<const BodyPart*, const BodyPart*> >
CollisionBody::collision(const CollisionBody& body1,
                         const CollisionBody& body2) {
    std::vector<std::pair<const BodyPart*, const BodyPart*> > collidingParts;
    for (const auto part1 : body1.bodyParts()) {
        for (const auto part2 : body2.bodyParts()) {
            if (part1.active() || part2.active()) {
                if (BodyPart::collision(part1, part2))
                    collidingParts.push_back(std::pair(&part1, &part2));
            }
        }
    }
    return collidingParts;
}

void CollisionBody::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
    states.texture = m_texture;
    target.draw(m_vertices, states);
}

}  // namespace VVipers