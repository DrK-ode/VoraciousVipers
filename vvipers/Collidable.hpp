#ifndef VVIPERS_COLLIDABLE_HPP
#define VVIPERS_COLLIDABLE_HPP

#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class CollidablePart {
  public:
    CollidablePart() : isActive(false), isSymmetric(false) {}
    void updateEdgesAndNormals() {
        updateEdges();
        updateNormals();
    }
    static bool collision(const CollidablePart&, const CollidablePart&);

    /** Only collisions involving at least one active colliding part are tried.
     * Passive parts are, e.g. walls, and other parts that won't move into other
     * parts.
     **/
    bool isActive;
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
    static bool collision(const Collidable&, const Collidable&);

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