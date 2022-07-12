#ifndef VVIPERS_COLLIDABLE_HPP
#define VVIPERS_COLLIDABLE_HPP

#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class CollidablePart {
  public:
    CollidablePart()
        : m_hasBeenModified(true), m_isActive(false), m_isSymmetric(false) {}
    void active(bool active) { m_isActive = active; }
    bool active() const { return m_isActive; }
    void label(const std::string& label) { m_label = label; }
    std::string label() const { return m_label; }
    size_t numberOfNodes() const { return m_nodes.size(); }
    void symmetric(bool symmetric) { m_isSymmetric = symmetric; }
    bool symmetric() const { return m_isSymmetric; }
    void resize(size_t s) {
        m_edges.resize(s);
        m_nodes.resize(s);
        m_normals.resize(s);
    }
    void node(int index, const Vec2& node) {
        m_nodes[index] = node;
        m_hasBeenModified = true;
    }
    Vec2 node(int index) const { return m_nodes[index]; }
    void update() {
        if (m_hasBeenModified) {
            updateEdges();
            updateNormals();
            m_hasBeenModified = false;
        }
    }
    static bool collision(const CollidablePart&, const CollidablePart&);

    /** Only collisions involving at least one active colliding part are tried.
     * Passive parts are, e.g. walls, and other parts that won't move into other
     * parts.
     **/
  protected:
    bool m_hasBeenModified;
    bool m_isActive;
    bool m_isSymmetric;
    std::string m_label;
    std::vector<Vec2> m_edges;
    std::vector<Vec2> m_nodes;
    std::vector<Vec2> m_normals;

    virtual void updateEdges();
    virtual void updateNormals();
};

class CollidableRect : public CollidablePart {
  public:
    CollidableRect(const Vec2& pos, const Vec2& size);
};

class Collidable {
  public:
    Collidable() : m_hasActivePart(false) {}
    virtual const std::vector<CollidablePart>& parts() const {
        return m_collidableParts;
    }
    static std::vector<std::pair<CollidablePart const*, CollidablePart const*> >
    collision(const Collidable&, const Collidable&);

  protected:
    virtual void updateCollidable() {
        updateNodes();
        for (auto& part : m_collidableParts) {
            if (part.active())
                m_hasActivePart = true;
            part.update();
        }
    }
    virtual void updateNodes() = 0;

    std::vector<CollidablePart> m_collidableParts;
    bool m_hasActivePart;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLIDABLE_HPP
