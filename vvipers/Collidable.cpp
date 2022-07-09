#include <limits>
#include <vvipers/Collidable.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

void projectionsMinMax(const std::vector<Vec2>& vectors, const Vec2& axis,
                       double& minimum, double& maximum) {
    minimum = std::numeric_limits<double>::max();
    maximum = std::numeric_limits<double>::lowest();
    for (const auto& v : vectors) {
        double projection = v.projectionScalar(axis);
        minimum = projection < minimum ? projection : minimum;
        maximum = projection > maximum ? projection : maximum;
    }
}

bool CollidablePart::collision(const CollidablePart& part1,
                               const CollidablePart& part2) {
    const auto axes = part1.normals.size() < part2.normals.size()
                          ? part1.normals
                          : part2.normals;
    for (auto& axis : axes) {
        double min1, max1, min2, max2;
        projectionsMinMax(part1.nodes, axis, min1, max1);
        projectionsMinMax(part2.nodes, axis, min2, max2);
        if (max1 < min2 || max2 < min1)
            return false;
    }
    return true;
}

void CollidablePart::updateEdges() {
    // Stores the edges in clockwise order starting from the top-right
    edges.resize(nodes.size());
    auto node1 = nodes.cbegin();
    auto node2 = node1 + 1;
    auto edge = edges.begin();
    while (node2 != nodes.end())
        *edge++ = *node2++ - *node1++;
    *edge = nodes.front() - nodes.back();
}

void CollidablePart::updateNormals() {
    if (isSymmetric)
        normals.resize(edges.size() / 2);
    else
        normals.resize(edges.size());
    for (int i = 0; i < normals.size(); ++i)
        normals[i] = edges[i].perpVec().normalize();
}

bool Collidable::collision(const Collidable& obj1, const Collidable& obj2) {
    for (const auto& part1 : obj1.parts()) {
        for (const auto& part2 : obj2.parts()) {
            if (part1.isActive || part2.isActive){
                logInfo("is active", " ", part1.isActive, " ", part2.isActive);
                if (CollidablePart::collision(part1, part2))
                    return true;
            }
        }
    }
    return false;
}

}  // namespace VVipers