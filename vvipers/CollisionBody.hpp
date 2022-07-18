#ifndef VVIPERS_COLLISIONBODY_HPP
#define VVIPERS_COLLISIONBODY_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>
#include <vvipers/BodyPart.hpp>
#include <vvipers/Vec2.hpp>

namespace VVipers {

/** CollisionBody is built around a VertexArray but also divides the vertices
 * into BodyParts which must be convex in order for the collision detection to
 * work. The vertices can be set arbitrarily and the body parts are later
 * assigned by specifying which consequetive range of vertices are one body
 * part. No parts of a CollisionBody will ever be checked for collisions between
 * each other. **/
class CollisionBody : public sf::Drawable {
  public:
    CollisionBody();
    bool isActive() const { return m_hasActivePart; }
    static std::vector<std::pair<const BodyPart*, const BodyPart*> > collision(
        const CollisionBody&, const CollisionBody&);
    void clear();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void resize(size_t s) { m_vertices.resize(s); }
    size_t size() const { return m_vertices.getVertexCount(); }
    void setTexture(const sf::Texture* texture) { m_texture = texture; }
    sf::Vertex getVertex(int i) const { return m_vertices[i]; }
    void setVertex(int i, const sf::Vertex& v) { m_vertices[i] = v; }
    void appendVertex(const sf::Vertex& v) { m_vertices.append(v); }

    void assignBodyParts(size_t beginIndex, size_t length,
                         size_t nodesPerBodyPart, const std::string& label = "",
                         int sharedNodes = 0, bool active = false,
                         bool symmetric = false);
    const std::vector<BodyPart>& bodyParts() const { return m_bodyParts; }

  protected:
    bool m_hasActivePart;
    std::vector<BodyPart> m_bodyParts;
    sf::VertexArray m_vertices;
    const sf::Texture* m_texture;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISIONBODY_HPP
