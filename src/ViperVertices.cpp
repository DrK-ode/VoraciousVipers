#include "ViperVertices.hpp"

#include "config.hpp"
#include "VectorMath.hpp"
#include "debug.hpp"

const size_t ViperVertices::s_nHeadVertices(10);
const size_t ViperVertices::s_nBodyVertices(8);
const size_t ViperVertices::s_nTailVertices(3);

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
    vertexPtr = &m_vertices[0];
    numberOfVertices = s_nHeadVertices;
    states.texture = &m_headTexture;
    target.draw(vertexPtr, numberOfVertices, sf::TriangleStrip, states);

    // Draw body
    // Each segment, except the head, shares the two first vertices with the
    // previous segment
    vertexPtr += (numberOfVertices - 2);
    // All that is not head or tail must be body (taking shared vertices into
    // account)
    numberOfVertices =
        m_vertices.size() - (s_nHeadVertices - 2) - (s_nTailVertices - 2);
    states.texture = &m_bodyTexture;
    target.draw(vertexPtr, numberOfVertices, sf::TriangleStrip, states);

    // Draw tail
    // Each segment, except the head, shares the two first vertices with the
    // previous segment
    vertexPtr += (numberOfVertices - 2);
    numberOfVertices = s_nTailVertices;
    states.texture = &m_tailTexture;
    target.draw(vertexPtr, numberOfVertices, sf::TriangleStrip, states);
}

void ViperVertices::setHeadVertices(TrackPoint* tp_front, TrackPoint* tp_back,uint32_t nPtsPerSeg, float segmentWidth, float segmentLength,
                                    sf::Vertex array[]) {
    float skel_y[s_nHeadVertices / 2];
    skel_y[0] = 0.f;
    skel_y[1] = 0.625f * segmentLength;
    skel_y[2] = 95.f / 120.f * segmentLength;
    skel_y[3] = 115.f / 120.f * segmentLength;
    skel_y[4] = segmentLength;
    float skel_x[s_nHeadVertices / 2];
    skel_x[0] = 0.125f * segmentWidth;
    skel_x[1] = 0.45f * segmentWidth;
    skel_x[2] = 0.45f * segmentWidth;
    skel_x[3] = 0.2f * segmentWidth;
    skel_x[4] = 0.2f * segmentWidth;

    TrackPoint* spine[s_nHeadVertices / 2];
    for (int i = 0; i < s_nHeadVertices / 2; ++i) {
        spine[i] =
            tp_front->step(nPtsPerSeg * skel_y[i] / segmentLength);
    }

    Vec2f arm[s_nHeadVertices / 2];
    for (int i = 0; i < s_nHeadVertices / 2; ++i)
        arm[i] =
            (*(spine[i]->next()) - *(spine[i])).perpVec().normalize(skel_x[i]);
    /* A point after the last TrackPoint is guaranteed since there will be
     * either a body segment or tail coming. */

    // Set triangles vertices
    for (int i = 0; i < s_nHeadVertices; ++i) {
        array[i].position = *spine[i / 2] - (1 - 2 * (i % 2)) * arm[i / 2];
        array[i].color = m_color;
    }

    // Set texture coordinates
    Vec2 textureSize(m_headTexture.getSize());
    for (int i = 0; i < s_nHeadVertices; ++i) {
        array[i].texCoords = {-(1 - 2 * (i % 2)) * skel_x[i / 2],
                              skel_y[i / 2]};
        // Move texture anchor point
        array[i].texCoords.x += segmentWidth / 2;
        // Scale the coordinates to the texture space
        array[i].texCoords.x *= textureSize.x / segmentWidth;
        array[i].texCoords.y *= textureSize.y / segmentLength;
    }
}

void ViperVertices::setBodyVertices(TrackPoint* tp_front, TrackPoint* tp_back,uint32_t nPtsPerSeg, float segmentWidth, float segmentLength,
                                    sf::Vertex array[]) {
    // The first two vertices are already created by the previous segment
    size_t nVertices = s_nBodyVertices - 2;
    float skel_x[nVertices / 2];
    skel_x[0] = 0.4f * segmentWidth;
    skel_x[1] = 0.4f * segmentWidth;
    skel_x[2] = 0.2f * segmentWidth;
    float skel_y[nVertices / 2];
    skel_y[0] = 0.25f * segmentLength;
    skel_y[1] = 0.75f * segmentLength;
    skel_y[2] = segmentLength;

    TrackPoint* spine[nVertices / 2];
    for (int i = 0; i < nVertices / 2; ++i) {
        spine[i] =
            tp_front->step(nPtsPerSeg * skel_y[i] / segmentLength);
    }

    Vec2f arm[nVertices / 2];
    for (int i = 0; i < nVertices / 2; ++i)
        arm[i] =
            (*(spine[i]->next()) - *(spine[i])).perpVec().normalize(skel_x[i]);
    // A point after the last TrackPoint is guaranteed because of the tail.

    // Set all the triangles vertices
    for (int i = 0; i < nVertices; ++i) {
        array[i].position = *spine[i / 2] - (1 - 2 * (i % 2)) * arm[i / 2];
        array[i].color = m_color;
    }

    // Prepare the texture coordinates
    //  The following code is identical for every body segment and should be
    //  bundled with the texture somehow
    Vec2 textureSize(m_bodyTexture.getSize());
    (array - 2)[0].texCoords = sf::Vector2f(12, 0);
    (array - 2)[1].texCoords = sf::Vector2f(28, 0);
    /*for (int i = 0; i < nVertices; ++i) {
        array[i].texCoords =
            sf::Vector2f(-(1 - 2 * (i % 2)) * skel_x[i / 2], skel_y[i / 2]);
        // Move texture anchor point
        array[i].texCoords.x += segmentWidth / 2;
        // Scale the coordinates to the texture space
        array[i].texCoords.x *= textureSize.x / segmentWidth;
        array[i].texCoords.y *= textureSize.y / segmentLength;
        tagInfo( array[i].position);
        tagInfo( array[i].texCoords);
    }*/
    array[0].texCoords = sf::Vector2f(4, 45);
    array[1].texCoords = sf::Vector2f(36, 45);
    array[2].texCoords = sf::Vector2f(4, 135);
    array[3].texCoords = sf::Vector2f(36, 135);
    array[4].texCoords = sf::Vector2f(12, 180);
    array[5].texCoords = sf::Vector2f(28, 180);
}

void ViperVertices::setTailVertices(TrackPoint* tp_front, TrackPoint* tp_back,uint32_t nPtsPerSeg, float segmentWidth, float segmentLength,
                                    sf::Vertex array[]) {
    // Draw tail
    array[0].position = *tp_back;
    array[0].color = m_color;

    // (array -2)[0].texCoords = sf::Vector2f(15,0);
    //(array -2)[1].texCoords = sf::Vector2f(25,0);
    array[0].texCoords = {m_tailTexture.getSize().x / 2.f,
                          1.f * m_tailTexture.getSize().y};
}

void ViperVertices::update(TrackPoint* tp_front, TrackPoint* tp_back, uint32_t nPtsPerSeg, float segmentWidth, float segmentLength) {
    size_t viperSize = tp_front->stepsUntil(tp_back); // We are counting intervals, not points
    uint32_t nHeadPts = nPtsPerSeg;
    size_t nBodySegments =
        std::max(0uL, (viperSize - nHeadPts) / nPtsPerSeg - 1);
    uint32_t nBodyPts = nBodySegments * nPtsPerSeg;
    uint32_t nTailPts = viperSize - nHeadPts - nBodyPts;
    // Sanity check
    if (nBodyPts > 0 && nTailPts < nPtsPerSeg) {
        tagError("Tail is too small even though is could be bigger.");
        throw std::runtime_error("Inacceptable tail size.");
    }
    size_t nVertices = s_nHeadVertices + (s_nBodyVertices - 2) * nBodySegments +
                       (s_nTailVertices - 2);
    m_vertices.resize(nVertices);

    TrackPoint* segFront = tp_front;
    TrackPoint* segBack = segFront->step(nPtsPerSeg);
    sf::Vertex* storagePtr = &m_vertices[0];
    setHeadVertices(segFront, segBack, nPtsPerSeg, segmentWidth, segmentLength, storagePtr);
    storagePtr += s_nHeadVertices;

    for (int i = 0; i < nBodySegments; ++i) {
        segFront = segBack;
        segBack = segFront->step(nPtsPerSeg);
        setBodyVertices(segFront, segBack, nPtsPerSeg, segmentWidth, segmentLength, storagePtr);
        storagePtr += (s_nBodyVertices - 2);
    }

    segFront = segBack;
    segBack = segFront->step(nTailPts);
    setTailVertices(segFront, segBack, nPtsPerSeg, segmentWidth, segmentLength, storagePtr);
}