#include <limits>
#include <vvipers/Collidable.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

void projectionsMinMax(const CollidablePart& part, const Vec2& axis,
                       double& minimum, double& maximum) {
    minimum = std::numeric_limits<double>::max();
    maximum = std::numeric_limits<double>::lowest();
    for (int i = 0; i < part.numberOfNodes(); ++i) {
        double projection = part.node(i).projectionScalar(axis);
        minimum = projection < minimum ? projection : minimum;
        maximum = projection > maximum ? projection : maximum;
    }
}

bool CollidablePart::collision(const CollidablePart& part1,
                               const CollidablePart& part2) {
    const auto axes = part1.m_normals.size() < part2.m_normals.size()
                          ? part1.m_normals
                          : part2.m_normals;
    for (auto& axis : axes) {
        double min1, max1, min2, max2;
        projectionsMinMax(part1, axis, min1, max1);
        projectionsMinMax(part2, axis, min2, max2);
        if (max1 < min2 || max2 < min1)
            return false;
    }
    return true;
}

void CollidablePart::updateEdges() {
    // Stores the edges in clockwise order starting from the top-right
    auto node1 = m_nodes.cbegin();
    auto node2 = node1 + 1;
    auto edge = m_edges.begin();
    while (node2 != m_nodes.end())
        *edge++ = *node2++ - *node1++;
    *edge = m_nodes.front() - m_nodes.back();
}

void CollidablePart::updateNormals() {
    if (m_isSymmetric)
        m_normals.resize(m_edges.size() / 2);
    else
        m_normals.resize(m_edges.size());
    for (int i = 0; i < m_normals.size(); ++i)
        m_normals[i] = m_edges[i].perpVec().normalize();
}

std::vector<std::pair<CollidablePart const*, CollidablePart const*> >
Collidable::collision(const Collidable& obj1, const Collidable& obj2) {
    std::vector<std::pair<CollidablePart const*, CollidablePart const*> >
        collidingParts;
    for (const auto& part1 : obj1.parts()) {
        for (const auto& part2 : obj2.parts()) {
            if (part1.active() || part2.active()) {
                logInfo("is active", " ", part1.active(), " ", part2.active());
                if (CollidablePart::collision(part1, part2))
                    collidingParts.push_back(std::pair(&part1, &part2));
            }
        }
    }
    return collidingParts;
}

}  // namespace VVipers