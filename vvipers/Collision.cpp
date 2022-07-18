#include <SFML/Graphics/RenderTarget.hpp>
#include <limits>
#include <iterator>
#include <vvipers/Collision.hpp>
#include <vvipers/GameEvent.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

void BodyPart::updateAxes() {
    // Stores the edges in clockwise order starting from the top-right
    m_axes.resize(m_isSymmetric ? m_nodes.size() / 2 : m_nodes.size());
    auto node1 = m_nodes.begin();
    auto node2 = node1 + 1;
    auto axis = m_axes.begin();
    while (axis != m_axes.end() - 1)
        *axis++ = (*node2++ - *node1++).perpVec();
    *axis = (m_nodes.front() - m_nodes.back()).perpVec();
}

void projectionsMinMax(const BodyPart& part, const Vec2& axis, double& minimum,
                       double& maximum) {
    minimum = std::numeric_limits<double>::max();
    maximum = std::numeric_limits<double>::lowest();
    auto nodes = part.nodes();
    for (int i = 0; i < part.numberOfNodes(); ++i) {
        double projection = nodes[i].projectionScalar(axis);
        minimum = projection < minimum ? projection : minimum;
        maximum = projection > maximum ? projection : maximum;
    }
}

bool BodyPart::collision(const BodyPart& part1, const BodyPart& part2) {
    const auto& axes =
        part1.axes().size() < part2.axes().size() ? part1.axes() : part2.axes();
    for (auto& axis : axes) {
        double min1, max1, min2, max2;
        projectionsMinMax(part1, axis, min1, max1);
        projectionsMinMax(part2, axis, min2, max2);
        if (max1 < min2 || max2 < min1)
            return false;
    }
    return true;
}

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

void CollisionDetector::checkForCollisions() const {
    // Collect all the bodies from the collidables
    std::vector<const CollisionBody*> bodies;
    for (auto c : m_collidables) {
        for (auto& b : c->collisionBodies()) {
            bodies.push_back(b);
        }
    }

    auto b1 = bodies.cbegin();
    while (b1 != bodies.cend()) {
        auto b2 = b1;
        ++b2;
        while (b2 != bodies.cend()) {
            const CollisionBody* A = *b1;
            const CollisionBody* B = *b2;
            const auto& partCollisions = CollisionBody::collision(*A, *B);
            for (auto partCollision : partCollisions) {
                CollisionEvent event(A, partCollision.first, B,
                                     partCollision.second);
                notify(&event);
            }
            ++b2;
        }
        ++b1;
    }
}

}  // namespace VVipers