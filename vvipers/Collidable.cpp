#include <vvipers/Collidable.hpp>

namespace VVipers {

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
    for (int i = 0; i < normals.size(); ++i )
        normals[i] = edges[i].perpVec();
}

}  // namespace VVipers