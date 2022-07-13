#include <limits>
#include <vvipers/Collidable.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

void projectionsMinMax(const CollidablePartInterface* part, const Vec2& axis,
                       double& minimum, double& maximum) {
    minimum = std::numeric_limits<double>::max();
    maximum = std::numeric_limits<double>::lowest();
    for (int i = 0; i < part->numberOfNodes(); ++i) {
        double projection = part->node(i).projectionScalar(axis);
        minimum = projection < minimum ? projection : minimum;
        maximum = projection > maximum ? projection : maximum;
    }
}

bool CollidablePartInterface::collision(const CollidablePartInterface* part1,
                                        const CollidablePartInterface* part2) {
    const auto& axes = part1->m_normals.size() < part2->m_normals.size()
                           ? part1->m_normals
                           : part2->m_normals;
    for (auto& axis : axes) {
        double min1, max1, min2, max2;
        projectionsMinMax(part1, axis, min1, max1);
        projectionsMinMax(part2, axis, min2, max2);
        if (max1 < min2 || max2 < min1)
            return false;
    }
    return true;
}

void CollidablePartInterface::node(int index, const Vec2& node) {
    m_nodes[index] = node;
    m_hasBeenModified = true;
}

void CollidablePartInterface::resize(size_t s) {
    m_nodes.resize(s);
    m_hasBeenModified = true;
}

void CollidablePartInterface::symmetric(bool symmetric) {
    if (m_isSymmetric != symmetric) {
        m_hasBeenModified = true;
        m_isSymmetric = symmetric;
    }
}

void CollidablePartInterface::update() {
    if (m_hasBeenModified) {
        updateEdges();
        updateNormals();
        m_hasBeenModified = false;
    }
}

void CollidablePartInterface::updateEdges() {
    // Stores the edges in clockwise order starting from the top-right
    m_edges.resize(m_nodes.size());
    auto node1 = m_nodes.cbegin();
    auto node2 = node1 + 1;
    auto edge = m_edges.begin();
    while (node2 != m_nodes.end())
        *edge++ = *node2++ - *node1++;
    *edge = m_nodes.front() - m_nodes.back();
}

void CollidablePartInterface::updateNormals() {
    if (m_isSymmetric)
        m_normals.resize(m_edges.size() / 2);
    else
        m_normals.resize(m_edges.size());
    for (int i = 0; i < m_normals.size(); ++i)
        m_normals[i] = m_edges[i].perpVec().normalize();
}

CollidableRect::CollidableRect(const Vec2& pos, const Vec2& size,
                               std::string label) : m_position(pos), m_size(size) {
    m_label = label;
    m_isSymmetric = true;
    m_nodes.resize(4);
    update();
}

void CollidableRect::update(){
    m_nodes[0] = m_position;
    m_nodes[1] = m_position + Vec2( m_size.x, 0);
    m_nodes[2] = m_position + m_size;
    m_nodes[3] = m_position + Vec2(0, m_size.y );
    // Call the base class to update the edges and normals
    CollidablePartInterface::update();
}

void Collidable::updateCollidable() {
    updateNodes();
    for (auto part : m_collidableParts) {
        if (part->active())
            m_hasActivePart = true;
        part->update();
    }
}

std::vector<
    std::pair<const CollidablePartInterface*, const CollidablePartInterface*> >
Collidable::collision(const Collidable& obj1, const Collidable& obj2) {
    std::vector<std::pair<const CollidablePartInterface*,
                          const CollidablePartInterface*> >
        collidingParts;
    for (const auto part1 : obj1.parts()) {
        for (const auto part2 : obj2.parts()) {
            if (part1->active() || part2->active()) {
                if (CollidablePartInterface::collision(part1, part2))
                    collidingParts.push_back(std::pair(part1, part2));
            }
        }
    }
    return collidingParts;
}

}  // namespace VVipers