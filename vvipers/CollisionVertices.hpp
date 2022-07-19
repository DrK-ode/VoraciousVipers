#ifndef VVIPERS_COLLISIONVERTICES_HPP
#define VVIPERS_COLLISIONVERTICES_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <string>
#include <vector>
#include <vvipers/CollisionBody.hpp>

namespace VVipers {

class Bodypart;

/** CollisionVertices is built around a VertexArray and implements
 * CollisionBody. It also also divides the vertices into BodyParts which must be
 * convex in order for the collision detection to work. The vertices can be set
 * arbitrarily and the body parts are later assigned by specifying which
 * consequetive range of vertices are one body part. No parts of a CollisionBody
 * will ever be checked for collisions between each other. **/

class CollisionVertices : public sf::Drawable, public CollisionBody {
  public:
    CollisionVertices();
    ~CollisionVertices();

    void clear();
    void clearBodyParts();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
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
    const std::vector<const Bodypart*> bodyparts() const { return m_bodyParts; }
    sf::Rect<double> rectangularBounds() const override {
        return sf::Rect<double>(m_vertices.getBounds());
    }

  protected:
    std::vector<const Bodypart*> m_bodyParts;
    sf::VertexArray m_vertices;
    const sf::Texture* m_texture;
};

}  // namespace VVipers

#endif  // VVIPERS_COLLISIONVERTICES_HPP
