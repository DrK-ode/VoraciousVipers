#include <limits>
#include <vvipers/BodyPart.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

BodyPart::BodyPart(const std::vector<Vec2>& nodes, const std::string& label,
                   bool active, bool symmetric, sf::PrimitiveType vertexOrder)
    : m_nodes(nodes),
      m_label(label),
      m_isActive(active),
      m_isSymmetric(symmetric) {
    updateAxes(vertexOrder);
}

void BodyPart::updateAxes(sf::PrimitiveType vertexOrder) {
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
}

/** Assuming N nodes are stored in TriangleFan order, the edges needed are
 * those connecting nodes:
 * N - 1
 * (n) - (n+1),
 * i.e. the edges between every node + the edge case in the beginning. The
 * first node being the centre is ignored. The edges themselves are not
 * stored, it's the normal vectors we need.
 **/
void BodyPart::updateAxesTriangleFan() {
    m_axes.resize(m_isSymmetric ? m_nodes.size() / 2 : m_nodes.size());

    // Special case in the beginning
    m_axes.push_back((m_nodes[1] - m_nodes.back()).perpVec());
    // If the shape is symmetric, only half of the edges are needed.
    auto nodeStop = m_isSymmetric ? m_nodes.begin() + 1 + m_nodes.size() / 2
                                  : m_nodes.end();
    // Edge connecting consequetive nodes
    for (auto node = m_nodes.begin() + 1; node != nodeStop; ++node)
        m_axes.push_back((*(node + 1) - *node).perpVec());
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
void BodyPart::updateAxesTriangleStrip() {
    m_axes.resize(m_isSymmetric ? m_nodes.size() / 2 : m_nodes.size());
    // Special case in the beginning
    m_axes.push_back((m_nodes[1] - m_nodes[0]).perpVec());
    // Edge connecting odd nodes
    for (auto node = m_nodes.begin() + 3; node < m_nodes.end(); node += 2)
        m_axes.push_back((*node - *(node - 2)).perpVec());
    /** If the shape is marked as symmetric, only half of the edges are needed.
     * **/
    if (m_isSymmetric)
        return;
    // Edge connecting even nodes
    for (auto node = m_nodes.begin() + 2; node < m_nodes.end(); node += 2)
        m_axes.push_back((*node - *(node - 2)).perpVec());
    // Special case in the end
    m_axes.push_back(*(m_nodes.rbegin() + 1) - m_nodes.back());
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

}  // namespace VVipers