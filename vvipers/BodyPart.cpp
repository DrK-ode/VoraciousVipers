#include <vvipers/BodyPart.hpp>
#include <limits>

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

}  // namespace VVipers