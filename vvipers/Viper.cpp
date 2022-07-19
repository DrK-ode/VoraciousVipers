#include <SFML/Graphics/RenderTarget.hpp>
#include <exception>
#include <vvipers/Vec2.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/ViperSketch.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;
const Time Viper::s_headTemporalLength(1s);
const Time Viper::s_bodyTemporalLength(1s);
const Time Viper::s_tailTemporalLength(1s);

const double Viper::s_nominalSpeed(60.);

Viper::Viper()
    : m_acc(0.),
      m_speed(s_nominalSpeed),
      m_growth(0),
      m_headPoint(nullptr),
      m_mainColor(sf::Color::Green) {
    loadTextures();
}

void Viper::growth(const Time& g) { m_growth += g; }

double Viper::length() const {
    return m_track.length(m_headPoint->getTime(),
                          m_headPoint->getTime() - m_temporalLength);
}

void Viper::onNotify(const GameEvent* event) {
    if (event->type() != GameEvent::EventType::Update)
        throw std::runtime_error("Wrong event type sent to Viper.");
    update(static_cast<const UpdateEvent*>(event)->elapsedTime);
}

// Each initial segment will get the same length and be setup in a line from the
// start coordinates to the end coordinates. The track will be prepared assuming
// nominal speed. Tail at from-vector, head at to-vector.
void Viper::setup(const Vec2& headPosition, double angle,
                  const Time& viperTemporalLength) {
    const double viperLength = toSeconds(viperTemporalLength) * s_nominalSpeed;
    m_angle = angle;
    Vec2 vipVec =
        viperLength * Vec2(cos(degToRad(angle)), sin(degToRad(angle)));
    Vec2 dL = vipVec / viperLength;
    Time tailTime = seconds(0);
    Time headTime = tailTime + viperTemporalLength;
    m_temporalLength = (headTime - tailTime);
    size_t numberOfPoints = 60 * toSeconds(m_temporalLength) + 1;  // 60 FPS

    // Find all the positions the Viper segments will move through
    m_track.clear();
    for (int i = 0; i < numberOfPoints; ++i) {
        m_track.create_back(
            headPosition - dL * i,
            headTime - (m_temporalLength * i) / (numberOfPoints - 1));
    }

    m_headPoint = m_track.front();
}

TrackPoint* Viper::createNextHeadTrackPoint(Time elapsedTime) {
    double dx = m_speed * toSeconds(elapsedTime) * cos(degToRad(m_angle));
    double dy = m_speed * toSeconds(elapsedTime) * sin(degToRad(m_angle));
    Vec2 advance(dx, dy);
    return m_track.create_front(*m_headPoint + advance,
                                m_headPoint->getTime() + elapsedTime);
}

void Viper::cleanUpTrailingTrackPoints() {
    // Removes any point(s) the whole Viper has passed through
    Time tailTime = m_headPoint->getTime() - m_temporalLength;
    // Find the last two needed trackpoints
    TrackPoint* afterTail = m_track.back();
    while (afterTail && afterTail->getTime() <= tailTime)
        afterTail = afterTail->prev();
    if (!afterTail)
        throw std::runtime_error(
            "About to remove all TrackPoints, this cannot be right.");
    TrackPoint* beforeTail = afterTail->next();

    // Remove the rest
    while (beforeTail != m_track.back()) {
        m_track.pop_back();
    }
}

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_headBody, states);
    target.draw(m_bodyBody, states);
    target.draw(m_tailBody, states);
}

void Viper::grow(const Time& elapsedTime) {
    // Limit the growth to how much time that has passed
    Time actualGrowth = std::min(m_growth, elapsedTime);
    m_temporalLength += actualGrowth;
    m_growth -= actualGrowth;
}

void Viper::loadTextures() {
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
    m_headBody.setTexture(&m_headTexture);
    m_bodyBody.setTexture(&m_bodyTexture);
    m_tailBody.setTexture(&m_tailTexture);
}

void Viper::steer(const SteeringEvent* orders) {
    m_angle += orders->deltaAngle;
}

void Viper::update(const Time& elapsedTime) {
    m_headPoint = createNextHeadTrackPoint(elapsedTime);
    grow(elapsedTime);
    updateBodies();
    cleanUpTrailingTrackPoints();
}

void Viper::updateBodies() {
    if (m_temporalLength < s_headTemporalLength + s_tailTemporalLength)
        throw std::runtime_error("Viper is too small");

    Time headLength = s_headTemporalLength;
    Time tailLength = s_tailTemporalLength;
    Time bodyLength = m_temporalLength - headLength - tailLength;

    Time headFront = m_headPoint->getTime();
    Time bodyFront = headFront - headLength;
    Time tailFront = bodyFront - bodyLength;

    const size_t nHeadNodes = ViperSketch::headNodes().size();
    const size_t nBodyNodes = ViperSketch::bodyNodes().size();
    const size_t nTailNodes = ViperSketch::tailNodes().size();

    // HEAD
    updateBody(ViperPart::Head, headFront, headLength);
    // BODY
    updateBody(ViperPart::Body, bodyFront, bodyLength);
    // TAIL
    updateBody(ViperPart::Tail, tailFront, tailLength);
}

void Viper::updateBody(ViperPart part, const Time& timeFront,
                       const Time& temporalLength) {
    double width = 20;  // TODO:Adapt width depending on how streched the
                        // segment is, i.e., dL/dt
    CollisionVertices* body;
    const std::vector<Vec2>* sketch;
    Vec2 textureSize;
    int numberOfSegments = 1;
    Time segmentLength = temporalLength;
    switch (part) {
        case ViperPart::Head: {
            body = &m_headBody;
            sketch = &ViperSketch::headNodes();
            textureSize = m_headTexture.getSize();
            break;
        }
        case ViperPart::Body: {
            body = &m_bodyBody;
            sketch = &ViperSketch::bodyNodes();
            textureSize = m_bodyTexture.getSize();
            segmentLength = s_bodyTemporalLength;
            numberOfSegments = temporalLength / segmentLength;
            break;
        }
        case ViperPart::Tail: {
            body = &m_tailBody;
            sketch = &ViperSketch::tailNodes();
            textureSize = m_tailTexture.getSize();
            break;
        }
    }
    // Used to extend a segment when, e.g., growing
    Time leftOverLength = temporalLength - segmentLength * numberOfSegments;
    body->clear();
    int sketchIndex = 0;
    for (int segmentIndex = 0; segmentIndex < numberOfSegments;
         ++segmentIndex) {
        while (sketchIndex < sketch->size()) {
            Time L =
                part == ViperPart::Body and segmentIndex == numberOfSegments - 1
                    ? segmentLength + leftOverLength
                    : segmentLength;
            Time time = timeFront - segmentIndex * segmentLength -
                        (*sketch)[sketchIndex].y * L;
            Vec2 mid = m_track.position(time);
            Vec2 perp = m_track.direction(time).perpVec() *
                        (width * (*sketch)[sketchIndex].x);
            Vec2 position = mid + perp;
            Vec2 textCoords =
                (Vec2(0.5f, segmentIndex) + (*sketch)[sketchIndex]) *
                textureSize;
            body->appendVertex(sf::Vertex(position, m_mainColor, textCoords));
            ++sketchIndex;
        }
        sketchIndex = 2;
    }

    // Prepare CollisionParts
    switch (part) {
        case ViperPart::Head: {
            body->assignBodyParts(0, 4, 4, "ViperTip", 0, true);
            body->assignBodyParts(2, body->size() - 2, 4, "ViperHead", 2);
            break;
        }
        case ViperPart::Body:
        case ViperPart::Tail: {
            body->assignBodyParts(
                0, body->size(), 4,
                (part == ViperPart::Body) ? "ViperBody" : "ViperTail", 2);
            break;
        }
    }
}

}  // namespace VVipers