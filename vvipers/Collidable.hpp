#ifndef VVIPERS_COLLIDABLE_HPP
#define VVIPERS_COLLIDABLE_HPP

#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class CollidablePart {
  public:
    CollidablePart() : isSymmetric(false) {}
    void updateEdgesAndNormals() {
        updateEdges();
        updateNormals();
    }

    bool isSymmetric;
    std::string label;
    std::vector<Vec2> edges;
    std::vector<Vec2> nodes;
    std::vector<Vec2> normals;

  protected:
    virtual void updateEdges();
    virtual void updateNormals();
};

class Collidable {
  public:
    virtual const std::vector<CollidablePart>& parts() const {
        return m_collidableParts;
    }

  protected:
    virtual void updateCollidable() {
        updateNodes();
        for (auto& part : m_collidableParts)
            part.updateEdgesAndNormals();
    }
    virtual void updateNodes() = 0;

    std::vector<CollidablePart> m_collidableParts;
};

}  // namespace VVipers

#endif