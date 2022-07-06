#include <vvipers/ViperGraphics.hpp>

#include <vvipers/ViperSketch.hpp>

namespace VVipers {

using namespace std::chrono_literals;
const Time ViperGraphics::s_headTemporalLength(1s);
const Time ViperGraphics::s_bodyTemporalLength(1s);
const Time ViperGraphics::s_tailTemporalLength(1s);

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

void ViperGraphics::update(const Time& elapsedTime,
                           const ViperPhysics& viperPhys) {
    updateVertices(viperPhys.head()->getTime(), viperPhys.temporalLength(),
                   viperPhys.getTrack());
}
void ViperGraphics::updateVertices(const Time& headFront,
                                   const Time& temporalLength,
                                   const Track& timeTrack) {
    if (temporalLength < s_headTemporalLength + s_tailTemporalLength)
        throw std::runtime_error("Viper is too small");

    Time headLength = s_headTemporalLength;
    size_t numberOfBodySegments =
        std::max( 0., (temporalLength - headLength) /
                              s_bodyTemporalLength -
                          1.);
    Time bodyLength = s_bodyTemporalLength * numberOfBodySegments;
    Time tailLength = temporalLength - headLength - bodyLength;

    Time bodyFront = headFront - headLength;
    Time tailFront = bodyFront - bodyLength;

    infoTag();
    logInfo("Preparing head starting at: ", headFront,
            "s and ending at: ", (headFront - headLength), "s.");
    prepareHead(headFront, headLength, timeTrack);
    if (numberOfBodySegments > 0) {
        logInfo("Preparing body starting at: ", bodyFront,
                "s and ending at: ", (bodyFront - bodyLength),
                "s.");
        prepareBody(bodyFront, bodyLength, timeTrack, numberOfBodySegments);
    }
    logInfo("Preparing tail starting at: ", tailFront,
            "s and ending at: ", (tailFront - tailLength), "s.");
    prepareTail(tailFront, tailLength, timeTrack);
}

// Helper function since the prepare methods share most of the code
void prepareSegments(const Time& timeFront, const Time& temporalLength,
                     const Track& timeTrack,
                     const std::vector<Vec2>& relativePosistion,
                     const sf::Color& color, const sf::Texture& texture,
                     std::vector<sf::Vertex>& storage, uint32_t nSegments = 1) {
    Vec2 textureSize = texture.getSize();
    const size_t nVertPerSeg = relativePosistion.size();
    const size_t addPerSeg = nVertPerSeg - 2;
    const size_t nVertices = 2 + addPerSeg * nSegments;
    const Time temporalSegmentLength = temporalLength / nSegments;
    storage.resize(nVertices);

    double width = 20;  // TODO:Adapt width depending on how streched the segment
                       // is, i.e., dL/dt

    auto iter = storage.begin();
    // seg is the current segment index
    // i is the index of the width and length arrays
    // iter points to the current vertex
    for (int seg = 0; seg < nSegments; ++seg) {
        // Skip the two first vertices of any segment coming after the first one
        const int startIndex = seg > 0 ? 2 : 0;
        for (int i = startIndex; i < nVertPerSeg; ++i) {
            Time temporalPosition =
                timeFront -
                convertTime((seg + relativePosistion[i].y) * temporalSegmentLength);

            Vec2 midPosition = timeTrack.position(temporalPosition);
            Vec2 perpLength = timeTrack.direction(temporalPosition).perpVec() *
                               (width * relativePosistion[i].x);

            iter->position = midPosition + perpLength;
            iter->color = color;
            iter->texCoords =
                (Vec2(0.5f, seg) + relativePosistion[i]) * textureSize;
            ++iter;
        }
    }
}

void ViperGraphics::prepareHead(const Time& timeFront,
                                const Time& temporalLength,
                                const Track& timeTrack) {
    prepareSegments(timeFront, temporalLength, timeTrack, ViperSketch::headNodes(), m_color,
                    m_headTexture, m_headVertices);
}

void ViperGraphics::prepareBody(const Time& timeFront,
                                const Time& temporalLength,
                                const Track& timeTrack, uint32_t nSegments) {
    prepareSegments(timeFront, temporalLength, timeTrack, ViperSketch::bodyNodes(), m_color,
                    m_bodyTexture, m_bodyVertices, nSegments);
}

void ViperGraphics::prepareTail(const Time& timeFront,
                                const Time& temporalLength,
                                const Track& timeTrack) {
    prepareSegments(timeFront, temporalLength, timeTrack, ViperSketch::tailNodes(), m_color,
                    m_tailTexture, m_tailVertices);
}
}  // namespace VVipers