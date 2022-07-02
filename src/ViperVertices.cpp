#include "ViperVertices.hpp"

#include "VectorMath.hpp"
#include "config.hpp"
#include "debug.hpp"

ViperVertices::ViperVertices() : m_color(0x007700ff) {
    // Load image and split it into the different textures.
    sf::Image combinedTextureImage;
    combinedTextureImage.loadFromFile(RESOURCE_PATH VIPER_TEXTURE_FILE);
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
                           uint32_t nPtsPerSeg, float segmentWidth,
                           float segmentLength) {
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

    prepareHead(headFront, nHeadPts, segmentWidth, segmentLength);
    prepareBody(bodyFront, nPtsPerSeg, nBodySegments, segmentWidth,
                segmentLength);
    prepareTail(tailFront, nTailPts, segmentWidth, segmentLength);
}

void ViperVertices::prepareHead(TrackPoint* tp_front, uint32_t nPtsPerSeg,
                                float segmentWidth, float segmentLength) {
    const size_t nHeadVertices = 10;
    m_headVertices.resize(nHeadVertices);
    const Vec2f textureSize(m_bodyTexture.getSize());

    float relWidth[nHeadVertices / 2] = {0.125, 0.45, 0.45, 0.2, 0.2};
    float relLength[nHeadVertices / 2] = {0, 0.625, 95 / 120., 115 / 120., 1};

    for (int i = 0; i < nHeadVertices / 2; ++i) {
        const TrackPoint& spine = *tp_front->step(nPtsPerSeg * relLength[i]);
        /* A point after the last TrackPoint is guaranteed since there will be
         * either a body segment or tail coming. */
        Vec2f arm = (*spine.next() - spine)
                        .perpVec()
                        .normalize(segmentWidth * relWidth[i]);
        m_headVertices[2 * i].position = spine + arm;
        m_headVertices[2 * i + 1].position = spine - arm;

        m_headVertices[2 * i].color = m_color;
        m_headVertices[2 * i + 1].color = m_color;

        m_headVertices[2 * i].texCoords =
            Vec2f(0.5f - relWidth[i], relLength[i]) * textureSize;
        m_headVertices[2 * i + 1].texCoords =
            Vec2f(0.5f + relWidth[i], relLength[i]) * textureSize;
    }
}

void ViperVertices::prepareBody(TrackPoint* tp_front, uint32_t nPtsPerSeg,
                                uint32_t nBodySegments, float segmentWidth,
                                float segmentLength) {
    const size_t nVertPerSeg = 8;
    const size_t addPerSeg = nVertPerSeg - 2;
    const size_t nBodyVertices = 2 + addPerSeg * nBodySegments;
    m_bodyVertices.resize(nBodyVertices);
    const Vec2f textureSize(m_bodyTexture.getSize());
    float relWidth[nVertPerSeg / 2] = {0.2, 0.4, 0.4, 0.2};
    float relLength[nVertPerSeg / 2] = {0, 0.25, 0.75, 1};
    // Grab the two first points from the last part of the body and handlke them
    // seperately
    int index = 0;
    m_bodyVertices[index] = m_headVertices[m_headVertices.size() - 2];
    m_bodyVertices[index].texCoords =
        Vec2f(0.5f - relWidth[0], relLength[0]) * textureSize;
    ++index;
    m_bodyVertices[index] = m_headVertices[m_headVertices.size() - 1];
    m_bodyVertices[index].texCoords =
        Vec2f(0.5f + relWidth[0], relLength[0]) * textureSize;
    ++index;

    TrackPoint* spine = tp_front;
    for (int seg = 0; seg < nBodySegments; ++seg) {
        // i is the index of the width and length arrays but index is the index
        // of the current vertex
        for (int i = 1; i < nVertPerSeg / 2; ++i) {
            const TrackPoint& vertebra =
                *spine->step(nPtsPerSeg * relLength[i]);
            /* A point after the last TrackPoint is guaranteed since there will
             * be either a body segment or tail coming. */
            Vec2f arm = (*vertebra.next() - vertebra)
                            .perpVec()
                            .normalize(segmentWidth * relWidth[i]);
            m_bodyVertices[index].position = vertebra + arm;
            m_bodyVertices[index].color = m_color;
            m_bodyVertices[index].texCoords =
                Vec2f(0.5f - relWidth[i], relLength[i]+seg) * textureSize;
            ++index;
            m_bodyVertices[index].position = vertebra - arm;
            m_bodyVertices[index].color = m_color;
            m_bodyVertices[index].texCoords =
                Vec2f(0.5f + relWidth[i], relLength[i]+seg) * textureSize;
            ++index;
        }
        spine = spine->step(nPtsPerSeg);
    }
}

void ViperVertices::prepareTail(TrackPoint* tp_front, uint32_t nPts,
                                float segmentWidth, float segmentLength) {
    m_tailVertices.resize(3);
    // Grab the two first points from the last part of the body
    m_tailVertices[0] = m_bodyVertices[m_bodyVertices.size() - 2];
    m_tailVertices[1] = m_bodyVertices[m_bodyVertices.size() - 1];
    // Draw tail
    m_tailVertices[2].position = *tp_front->step(nPts);
    m_tailVertices[2].color = m_color;

    // The texture coordinates should not change?
    m_tailVertices[0].texCoords =
        sf::Vector2f(0.3 * m_tailTexture.getSize().x, 0);
    m_tailVertices[1].texCoords =
        sf::Vector2f(0.7 * m_tailTexture.getSize().x, 0);
    m_tailVertices[2].texCoords = sf::Vector2f(m_tailTexture.getSize().x / 2.f,
                                               m_tailTexture.getSize().y);
}