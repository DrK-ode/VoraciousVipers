#include "vvipers/ViperVertices.hpp"

#include <algorithm>
#include <vvipers/VectorMath.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

const float ViperVertices::s_segmentWidth(40);
const float ViperVertices::s_segmentLength(60);

ViperVertices::ViperVertices() : m_color(0x007700ff) {
    // Load image and split it into the different textures.
    sf::Image combinedTextureImage;
    combinedTextureImage.loadFromFile(VIPER_TEXTURE_FILE);
    sf::Vector2i imgSize(combinedTextureImage.getSize());
    Vec2i upperLeft;
    Vec2i rectSize(imgSize.x, imgSize.y / 3);

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

void ViperVertices::draw(sf::RenderTarget& target,
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

void ViperVertices::update(TrackPoint* tp_front, TrackPoint* tp_back,
                           uint32_t nPtsPerSeg) {
    // We are counting intervals, not points
    size_t viperSize = tp_front->stepsUntil(tp_back);
    uint32_t nHeadPts = nPtsPerSeg;
    size_t nBodySegments =
        std::max(0uL, (viperSize - nHeadPts) / nPtsPerSeg - 1);
    uint32_t nBodyPts = nBodySegments * nPtsPerSeg;
    uint32_t nTailPts = viperSize - nHeadPts - nBodyPts;

    // Find start and end of the different parts
    TrackPoint* headFront = tp_front;
    TrackPoint* headBack = headFront->step(nHeadPts);
    TrackPoint* bodyFront = headBack;
    TrackPoint* bodyBack = bodyFront->step(nBodyPts);
    TrackPoint* tailFront = bodyBack;
    TrackPoint* tailBack = tailFront->step(nTailPts);

    prepareHead(headFront, nHeadPts);
    prepareBody(bodyFront, nPtsPerSeg, nBodySegments);
    prepareTail(tailFront, nTailPts);
}

// Helper function since the prepare methods share most of the code
void prepareSegments(TrackPoint* tp_front, uint32_t nPtsPerSeg,
                     uint32_t nSegments, float segmentWidth,
                     float segmentLength, const std::vector<Vec2f>& relSize,
                     const sf::Color& color, const Vec2f textureSize,
                     const std::vector<sf::Vertex>* copySource,
                     std::vector<sf::Vertex>& storage) {
    const size_t nVertPerSeg = relSize.size();
    const size_t addPerSeg = nVertPerSeg - 2;
    const size_t nVertices = 2 + addPerSeg * nSegments;
    storage.resize(nVertices);

    int startIndex = 0;
    auto iter = storage.begin();
    if (copySource) {
        // Grab the two first points from the last part of the head and handle
        // them seperately
        std::copy(copySource->end() - 2, copySource->end(), iter);
        (iter++)->texCoords = (Vec2f(0.5f, 0) + relSize[0]) * textureSize;
        (iter++)->texCoords = (Vec2f(0.5f, 0) + relSize[1]) * textureSize;
        startIndex = 2;
    }

    TrackPoint* spine = tp_front;
    for (int seg = 0; seg < nSegments; ++seg) {
        // i is the index of the width and length arrays while the iterator
        // points to the current vertex
        for (int i = startIndex; i < nVertPerSeg; ++i) {
            const TrackPoint& vertebra =
                *spine->step(nPtsPerSeg * relSize[i].y);
            /* i is the index of the width and length arrays while the iterator
             * points to the current vertex. */
            /* Each segment will use the two last vertices from the previous
             * segment and thereby not adding those. */
            Vec2f arm = (vertebra.next() ? (*vertebra.next() - vertebra)
                                         : (vertebra - *vertebra.prev()))
                            .perpVec()
                            .normalize(-segmentWidth * relSize[i].x);
            iter->position = vertebra + arm;
            iter->color = color;
            iter->texCoords = (Vec2f(0.5f, seg) + relSize[i]) * textureSize;
            ++iter;
        }
        spine = spine->step(nPtsPerSeg);
        startIndex = 2;  // Always skip the first two vertices if we're doing
                         // several segments
    }
}

void ViperVertices::prepareHead(TrackPoint* tp_front, uint32_t nPtsPerSeg) {
    const size_t nSegments = 1;
    sf::Texture& texture = m_headTexture;
    std::vector<sf::Vertex>& storage = m_headVertices;
    const std::vector<sf::Vertex>* copySource = nullptr;

    std::vector<Vec2f> relSize;
    relSize.push_back({-0.125, 0});
    relSize.push_back({0.125, 0});
    relSize.push_back({-0.45, 0.625});
    relSize.push_back({0.45, 0.625});
    relSize.push_back({-0.45, 0.8});
    relSize.push_back({0.45, 0.8});
    relSize.push_back({-0.2, 0.95});
    relSize.push_back({0.2, 0.95});
    relSize.push_back({-0.2, 1});
    relSize.push_back({0.2, 1});

    prepareSegments(tp_front, nPtsPerSeg, nSegments, s_segmentWidth,
                    s_segmentLength, relSize, m_color, texture.getSize(),
                    copySource, storage);
}

void ViperVertices::prepareBody(TrackPoint* tp_front, uint32_t nPtsPerSeg,
                                uint32_t nSegments) {
    sf::Texture& texture = m_bodyTexture;
    std::vector<sf::Vertex>& storage = m_bodyVertices;
    const std::vector<sf::Vertex>* copySource = &m_headVertices;

    std::vector<Vec2f> relSize;
    relSize.push_back({-0.2, 0});
    relSize.push_back({0.2, 0});
    relSize.push_back({-0.4, 0.25});
    relSize.push_back({0.4, 0.25});
    relSize.push_back({-0.4, 0.75});
    relSize.push_back({0.4, 0.75});
    relSize.push_back({-0.2, 1});
    relSize.push_back({0.2, 1});

    prepareSegments(tp_front, nPtsPerSeg, nSegments, s_segmentWidth,
                    s_segmentLength, relSize, m_color, texture.getSize(),
                    copySource, storage);
}

void ViperVertices::prepareTail(TrackPoint* tp_front, uint32_t nPtsPerSeg) {
    sf::Texture& texture = m_tailTexture;
    std::vector<sf::Vertex>& storage = m_tailVertices;
    const std::vector<sf::Vertex>* copySource = &m_bodyVertices;
    const size_t nSegments = 1;

    std::vector<Vec2f> relSize;
    relSize.push_back({-0.2, 0});
    relSize.push_back({0.2, 0});
    relSize.push_back({-0.1, 0.5});
    relSize.push_back({0.1, 0.5});
    relSize.push_back({-0.01, 1});
    relSize.push_back({0.01, 1});

    prepareSegments(tp_front, nPtsPerSeg, nSegments, s_segmentWidth,
                    s_segmentLength, relSize, m_color, texture.getSize(),
                    copySource, storage);
}

}  // namespace VVipers