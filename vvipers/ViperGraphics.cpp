#include <vvipers/ViperGraphics.hpp>
#include <vvipers/ViperSketch.hpp>

namespace VVipers {

ViperGraphics::ViperGraphics() : m_color(0x007700ff) { loadTextures(); }

void ViperGraphics::loadTextures() {
    // Load image and split it into the different textures.
    sf::Image combinedTextureImage;
    combinedTextureImage.loadFromFile(VIPER_FILE_PATH);
    sf::Vector2i imgSize(combinedTextureImage.getSize());
    sf::Vector2i upperLeft;
    sf::Vector2i rectSize(imgSize.x, imgSize.y / 3);

    // Head texture
    upperLeft = {0, 0};
    m_headTexture.loadFromImage(combinedTextureImage,
                                sf::IntRect(upperLeft, rectSize));
    // Body texture
    upperLeft.y = rectSize.y;
    m_bodyTexture.loadFromImage(combinedTextureImage,
                                sf::IntRect(upperLeft, rectSize));
    m_bodyTexture.setRepeated(true);
    // Tail texture
    upperLeft.y = 2 * rectSize.y;
    m_tailTexture.loadFromImage(combinedTextureImage,
                                sf::IntRect(upperLeft, rectSize));
}

void ViperGraphics::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
    // Viper is drawn in three parts: head, tail and all body segments together
    const sf::Vertex* vertexPtr;
    size_t numberOfVertices;

    // Draw head
    states.texture = &m_headTexture;
    target.draw(&m_headVertices[0], m_headVertices.size(), sf::TriangleStrip,
                states);

    // Draw body
    states.texture = &m_bodyTexture;
    target.draw(&m_bodyVertices[0], m_bodyVertices.size(), sf::TriangleStrip,
                states);

    // Draw tail
    states.texture = &m_tailTexture;
    target.draw(&m_tailVertices[0], m_tailVertices.size(), sf::TriangleStrip,
                states);
}

void ViperGraphics::update(const ViperPhysics& viperPhys) {
    updateVertices(viperPhys.headNodes(), ViperSketch::headNodes(),
                   m_headTexture, m_headVertices);
    updateVertices(viperPhys.bodyNodes(), ViperSketch::bodyNodes(),
                   m_bodyTexture, m_bodyVertices);
    updateVertices(viperPhys.tailNodes(), ViperSketch::tailNodes(),
                   m_tailTexture, m_tailVertices);
}

// Helper function since the prepare methods share most of the code
void ViperGraphics::updateVertices(const std::vector<Vec2>& nodes,
                                   const std::vector<Vec2>& relativePositions,
                                   const sf::Texture& texture,
                                   std::vector<sf::Vertex>& vertices) {
    if (nodes.size() == 0)
        return;
    Vec2 textureSize = texture.getSize();
    const size_t nVertPerSeg = relativePositions.size();
    const size_t addPerSeg = nVertPerSeg - 2;
    const size_t nSegments = (nodes.size() + 2) / (nVertPerSeg - 2);
    vertices.resize(nodes.size());

    auto iterVert = vertices.begin();
    auto iterPosRight = nodes.cbegin();
    auto iterPosLeft = nodes.crbegin();
    auto iterRelRight = relativePositions.cbegin();
    auto iterRelLeft = relativePositions.crbegin();

    int seg = 0;
    while (iterVert != vertices.end()) {
        while (iterRelRight < iterRelLeft.base()) {
            iterVert->position = sf::Vector2f(*iterPosLeft++);
            iterVert->color = m_color;
            iterVert->texCoords =
                (Vec2(0.5f, seg) + *iterRelLeft++) * textureSize;
            ++iterVert;
            iterVert->position = sf::Vector2f(*iterPosRight++);
            iterVert->color = m_color;
            iterVert->texCoords =
                (Vec2(0.5f, seg) + *iterRelRight++) * textureSize;
            ++iterVert;
        }
        ++seg;
        iterRelRight = relativePositions.cbegin() + 1;
        iterRelLeft = relativePositions.crbegin() + 1;
    }
}

}  // namespace VVipers