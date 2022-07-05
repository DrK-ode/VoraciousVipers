#include "vvipers/ViperVertices.hpp"

#include <algorithm>
#include <vvipers/VectorMath.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

const sf::Time ViperVertices::s_headTemporalLength(sf::seconds(1));
const sf::Time ViperVertices::s_bodyTemporalLength(sf::seconds(1));
const sf::Time ViperVertices::s_tailTemporalLength(sf::seconds(1));

ViperVertices::ViperVertices() : m_color(0x007700ff) {
    // Load image and split it into the different textures.
    sf::Image combinedTextureImage;
    combinedTextureImage.loadFromFile(VIPER_FILE_PATH);
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

void ViperVertices::update(const sf::Time& headFront,
                           const sf::Time& temporalLength,
                           const Track& timeTrack) {
    if (temporalLength < s_headTemporalLength + s_tailTemporalLength)
        throw std::runtime_error("Viper is too small");

    sf::Time headLength = s_headTemporalLength;
    size_t numberOfBodySegments =
        std::max(0.f, (temporalLength - headLength).asSeconds() /
                              s_bodyTemporalLength.asSeconds() -
                          1);
    sf::Time bodyLength = s_bodyTemporalLength * float(numberOfBodySegments);
    sf::Time tailLength = temporalLength - headLength - bodyLength;

    sf::Time bodyFront = headFront - headLength;
    sf::Time tailFront = bodyFront - bodyLength;

    infoTag();
    logInfo("Preparing head starting at: ", headFront.asSeconds(), "s and ending at: ", (headFront-headLength).asSeconds() , "s.");
    prepareHead(headFront, headLength, timeTrack);
    if (numberOfBodySegments > 0) {
        logInfo("Preparing body starting at: ", bodyFront.asSeconds(), "s and ending at: ", (bodyFront-bodyLength).asSeconds() , "s.");
        prepareBody(bodyFront, bodyLength, timeTrack, numberOfBodySegments);
    }
    logInfo("Preparing tail starting at: ", tailFront.asSeconds(), "s and ending at: ", (tailFront-tailLength).asSeconds() , "s.");
    prepareTail(tailFront, tailLength, timeTrack);
}

// Helper function since the prepare methods share most of the code
void prepareSegments(const sf::Time& timeFront, const sf::Time& temporalLength,
                     const Track& timeTrack,
                     const std::vector<Vec2f>& relativePosistion,
                     const sf::Color& color, const sf::Texture& texture,
                     std::vector<sf::Vertex>& storage, uint32_t nSegments = 1) {
    Vec2f textureSize = texture.getSize();
    const size_t nVertPerSeg = relativePosistion.size();
    const size_t addPerSeg = nVertPerSeg - 2;
    const size_t nVertices = 2 + addPerSeg * nSegments;
    const sf::Time temporalSegmentLength = temporalLength / float(nSegments);
    storage.resize(nVertices);

    float width = 20;  // TODO:Adapt width depending on how streched the segment
                       // is, i.e., dL/dt

    auto iter = storage.begin();
    // seg is the current segment index
    // i is the index of the width and length arrays
    // iter points to the current vertex
    for (int seg = 0; seg < nSegments; ++seg) {
        // Skip the two first vertices of any segment coming after the first one
        const int startIndex = seg > 0 ? 2 : 0;
        for (int i = startIndex; i < nVertPerSeg; ++i) {
            sf::Time temporalPosition =
                timeFront -
                (seg + relativePosistion[i].y) * temporalSegmentLength;

            Vec2f midPosition = timeTrack.position(temporalPosition);
            Vec2f perpLength = timeTrack.direction(temporalPosition).perpVec() *
                               (width * relativePosistion[i].x);

            iter->position = midPosition + perpLength;
            iter->color = color;
            iter->texCoords =
                (Vec2f(0.5f, seg) + relativePosistion[i]) * textureSize;
            ++iter;
        }
    }
}

void ViperVertices::prepareHead(const sf::Time& timeFront,
                                const sf::Time& temporalLength,
                                const Track& timeTrack) {
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

    prepareSegments(timeFront, temporalLength, timeTrack, relSize, m_color,
                    m_headTexture, m_headVertices);
}

void ViperVertices::prepareBody(const sf::Time& timeFront,
                                const sf::Time& temporalLength,
                                const Track& timeTrack, uint32_t nSegments) {
    std::vector<Vec2f> relSize;
    relSize.push_back({-0.2, 0});
    relSize.push_back({0.2, 0});
    relSize.push_back({-0.4, 0.25});
    relSize.push_back({0.4, 0.25});
    relSize.push_back({-0.4, 0.75});
    relSize.push_back({0.4, 0.75});
    relSize.push_back({-0.2, 1});
    relSize.push_back({0.2, 1});

    prepareSegments(timeFront, temporalLength, timeTrack, relSize, m_color,
                    m_bodyTexture, m_bodyVertices, nSegments);
}

void ViperVertices::prepareTail(const sf::Time& timeFront,
                                const sf::Time& temporalLength,
                                const Track& timeTrack) {
    std::vector<Vec2f> relSize;
    relSize.push_back({-0.2, 0});
    relSize.push_back({0.2, 0});
    relSize.push_back({-0.1, 0.5});
    relSize.push_back({0.1, 0.5});
    relSize.push_back({-0.01, 1});
    relSize.push_back({0.01, 1});

    prepareSegments(timeFront, temporalLength, timeTrack, relSize, m_color,
                    m_tailTexture, m_tailVertices);
}

}  // namespace VVipers