#include <limits>
#include <tuple>
#include <vvipers/Bodypart.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

Bodypart::Bodypart(const std::vector<Vec2>& nodes, PartID_t id,
                   bool active, bool symmetric, sf::PrimitiveType vertexOrder)
    : m_nodes(nodes),
      partID(id),
      m_isActive(active),
      m_isSymmetric(symmetric) {
    update(vertexOrder);
}

void Bodypart::update(sf::PrimitiveType vertexOrder) {
    switch (vertexOrder) {
        case sf::PrimitiveType::TriangleStrip:
            updateAxesTriangleStrip();
            break;
        case sf::PrimitiveType::TriangleFan:
            updateAxesTriangleFan();
            break;
        default:
            throw std::runtime_error(
                "Illegal PrimitiveType for determining the vertex order.");
            break;
    }
    m_boundingRect = VVipers::rectangularBounds( m_nodes );
}

/** Assuming N nodes are stored in TriangleFan order, the edges needed are
 * those connecting nodes:
 * N - 1
 * (n) - (n+1),
 * i.e. the edges between every node + the edge case in the beginning. The
 * first node being the centre is ignored. The edges themselves are not
 * stored, it's the normal vectors we need.
 **/
void Bodypart::updateAxesTriangleFan() {
    m_axes.resize(m_isSymmetric ? (m_nodes.size() - 1) / 2 : m_nodes.size() - 1);

    // Special case in the beginning
    m_axes.push_back((m_nodes.back() - m_nodes[1]).perpVec());
    // If the shape is symmetric, only half of the edges are needed.
    auto nodeStop = m_isSymmetric ? m_nodes.begin() + 1 + m_nodes.size() / 2
                                  : m_nodes.end();
    // Edge connecting consequetive nodes
    for (auto node = m_nodes.begin() + 1; node != nodeStop; ++node)
        m_axes.push_back((*node - *(node + 1)).perpVec());
}

/** Assuming N nodes are stored in TriangleStrip order, the edges needed are
 * those connecting nodes:
 * 0-1,
 * (n) - (n+2),
 * (N-1) - N.
 * i.e. the edges between every second node + the edge cases in the
 * beginning and the end. The edges themselves are not stored, it's the
 * normal vectors we need.
 **/
void Bodypart::updateAxesTriangleStrip() {
    m_axes.clear();
    m_axes.reserve(m_isSymmetric ? m_nodes.size() / 2 : m_nodes.size());
    // Special case in the beginning
    m_axes.push_back((m_nodes[0] - m_nodes[1]).perpVec());
    // Edge connecting odd nodes
    for (auto node = m_nodes.begin() + 3; node < m_nodes.end(); node += 2)
        m_axes.push_back((*(node - 2) - *node).perpVec());
    /** If the shape is marked as symmetric, only half of the edges are needed.
     * **/
    if (m_isSymmetric)
        return;
    // Edge connecting even nodes
    for (auto node = m_nodes.begin() + 2; node < m_nodes.end(); node += 2)
        m_axes.push_back((*(node - 2) - *node).perpVec());
    // Special case in the end
    m_axes.push_back(*(m_nodes.rbegin() + 1) - m_nodes.back());
}

std::tuple<double, double> projectionsMinMax(const Bodypart* part,
                                            const Vec2& axis) {
    double minimum = std::numeric_limits<double>::max();
    double maximum = std::numeric_limits<double>::lowest();
    auto nodes = part->nodes();
    for (int i = 0; i < part->numberOfNodes(); ++i) {
        double projection = nodes[i].projectionScalar(axis);
        minimum = projection < minimum ? projection : minimum;
        maximum = projection > maximum ? projection : maximum;
    }
    return {minimum, maximum};
}

bool Bodypart::collision(const Bodypart* part1, const Bodypart* part2) {
    // Rough check with rectangular bounds
    if( ! part1->rectangularBounds().intersects( part2->rectangularBounds() ))
        return false;
    const auto& axes = part1->axes().size() < part2->axes().size()
                           ? part1->axes()
                           : part2->axes();
    for (auto& axis : axes) {
        auto [min1, max1] = projectionsMinMax(part1, axis);
        auto [min2, max2] = projectionsMinMax(part2, axis);
        if (max1 < min2 || max2 < min1)
            return false;
    }
    return true;
}

bool Bodypart::inside(const Vec2& point) const {
    for (auto& axis : m_axes) {
        auto [minimum, maximum] = projectionsMinMax(this, axis);
        double projection = point.projectionScalar(axis);
        if (maximum < projection || projection < minimum)
            return false;
    }
    return true;
}

}  // namespace VVipers