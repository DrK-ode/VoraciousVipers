#ifndef VVIPERS_COLLIDABLE_HPP
#define VVIPERS_COLLIDABLE_HPP

#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers {

class CollidablePartInterface {
  public:
    CollidablePartInterface()
        : m_hasBeenModified(true), m_isActive(false), m_isSymmetric(false) {}
    /** Only set needed parts as active for performance reasons. **/
    void active(bool active) { m_isActive = active; }
    /** An active object might move in to things. A passive object will not be
     * checked for collisions with other passive objects. **/
    bool active() const { return m_isActive; }
    /** A label helps identify the part in a multi-part object **/
    void label(const std::string& label) { m_label = label; }
    /** A label helps identify the part in a multi-part object **/
    std::string label() const { return m_label; }
    /** Only getter is available from the base class **/
    Vec2 node(int index) const { return m_nodes[index]; }
    /** The number of nodes will depend on the derived class **/
    size_t numberOfNodes() const { return m_nodes.size(); }
    /** If the part is symmetric only half the number of normals are needed. **/
    bool symmetric() const { return m_isSymmetric; }
    /** the update method must make sure that the edges and normals (and
     * possibly other members) are in sync with the nodes. **/
    virtual void update();
    /** Only collisions involving at least one active colliding part are tried.
     * Passive parts are, e.g. walls, and other parts that won't move into other
     * parts. **/
    static bool collision(const CollidablePartInterface*,
                          const CollidablePartInterface*);

  protected:
    void resize(size_t s);
    void node(int index, const Vec2& node);
    void symmetric(bool symmetric);

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

class CollidableNodes : public CollidablePartInterface {
  public:
    using CollidablePartInterface::node;
    using CollidablePartInterface::resize;
    using CollidablePartInterface::symmetric;
};

class CollidableRect : public CollidableNodes {
  public:
    CollidableRect(const Vec2& pos, const Vec2& size,
                   std::string = "Rectangle");
    void move(const Vec2& delta) { m_position += delta; }
    Vec2 size() const { return m_size; }
    void size(const Vec2& size) { m_size = size; }
    Vec2 position() const { return m_position; }
    void position(const Vec2& position) { m_position = position; }
    void update();

  protected:
    Vec2 m_position;
    Vec2 m_size;
};

class Collidable {
  public:
    Collidable() : m_hasActivePart(false) {}
    virtual const std::vector<CollidablePartInterface*>& parts() const {
        return m_collidableParts;
    }
    static std::vector<std::pair<const CollidablePartInterface*,
                                 const CollidablePartInterface*> >
    collision(const Collidable&, const Collidable&);

  protected:
    virtual void updateCollidable();
    virtual void updateNodes() = 0;

    std::vector<CollidablePartInterface*> m_collidableParts;
    bool m_hasActivePart;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLIDABLE_HPP
