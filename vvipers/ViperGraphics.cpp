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
    const int numberOfHeadNodes = ViperSketch::headNodes().size();
    m_headVertices.resize(numberOfHeadNodes);
    auto headVertex = m_headVertices.begin();

    // The first body segment will add all the nodes while the subsequent ones
    // does not add the first two.
    int numberOfBodyNodes = ViperSketch::bodyNodes().size();
    const int numberOfBodyParts = viperPhys.parts().size() - 2;
    int bodyIndex = 0;
    m_bodyVertices.resize(numberOfBodyParts * (numberOfBodyNodes - 2) + 2);
    auto bodyVertex = m_bodyVertices.begin();

    const int numberOfTailNodes = ViperSketch::tailNodes().size();
    m_tailVertices.resize(numberOfTailNodes);
    auto tailVertex = m_tailVertices.begin();

    for (const auto& part : viperPhys.parts()) {
        if (part.label() == "ViperHead") {
            updateVertices(part, ViperSketch::headNodes(), m_headTexture,
                           headVertex, numberOfHeadNodes);
        } else if (part.label() == "ViperBody") {
            updateVertices(part, ViperSketch::bodyNodes(), m_bodyTexture,
                           bodyVertex, numberOfBodyNodes, bodyIndex++);
            // All subsequent body segments will add two less vertices
            numberOfBodyNodes = ViperSketch::bodyNodes().size() - 2;
        } else if (part.label() == "ViperTail") {
            updateVertices(part, ViperSketch::tailNodes(), m_tailTexture,
                           tailVertex, numberOfTailNodes);
        }
    }
}

// Helper function since the prepare methods share most of the code
void ViperGraphics::updateVertices(
    const CollidablePart& part, const std::vector<Vec2>& relativePositions,
    const sf::Texture& texture, std::vector<sf::Vertex>::iterator& iterVertex,
    int numberOfNodes, int segmentIndex) {
    Vec2 textureSize = texture.getSize();

    int nodeIndexRight = 0;
    int nodeIndexLeft = part.numberOfNodes() - 1;
    auto iterRelRight = relativePositions.cbegin();
    auto iterRelLeft = relativePositions.crbegin();

    for (int i = numberOfNodes; i < relativePositions.size(); i += 2) {
        ++iterRelRight;
        ++iterRelLeft;
        ++nodeIndexRight;
        --nodeIndexLeft;
    }

    while (iterRelRight < iterRelLeft.base()) {
        iterVertex->position = sf::Vector2f(part.node(nodeIndexRight++));
        iterVertex->color = m_color;
        iterVertex->texCoords =
            (Vec2(0.5f, segmentIndex) + *iterRelRight++) * textureSize;
        ++iterVertex;
        iterVertex->position = sf::Vector2f(part.node(nodeIndexLeft--));
        iterVertex->color = m_color;
        iterVertex->texCoords =
            (Vec2(0.5f, segmentIndex) + *iterRelLeft++) * textureSize;
        ++iterVertex;
    }
}

}  // namespace VVipers