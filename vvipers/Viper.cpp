#include <SFML/Graphics/RenderTarget.hpp>
#include <exception>
#include <vvipers/Vec2.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/ViperSketch.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;
const Time Viper::s_headTemporalLength(0.5s);
const Time Viper::s_bodyTemporalLength(0.5s);
const Time Viper::s_tailTemporalLength(0.5s);

const double Viper::s_nominalSpeed(60.);

Viper::Viper()
    : m_state(ViperAlive),
      m_acc(0.),
      m_speed(s_nominalSpeed),
      m_growth(0),
      m_headPoint(nullptr),
      m_mainColor(sf::Color::Green),
      m_headBody(ViperHead),
      m_bodyBody(ViperBody),
      m_tailBody(ViperTail) {
    loadTextures();
}

void Viper::die(const Time& elapsedTime) {
    m_temporalLength -= elapsedTime;
    if (m_temporalLength <= seconds(0))
        m_state = ViperDead;
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
// void Viper::setup(const Vec2& headPosition, double angle,
//                   const Time& viperTemporalLength) {
//     const double viperLength = toSeconds(viperTemporalLength) * s_nominalSpeed;
//     m_angle = angle;
//     Vec2 vipVec =
//         viperLength * Vec2(cos(degToRad(angle)), sin(degToRad(angle)));
//     Vec2 dL = vipVec / viperLength;
//     Time tailTime = seconds(0);
//     Time headTime = tailTime + viperTemporalLength;
//     m_temporalLength = (headTime - tailTime);
//     size_t numberOfPoints = 60 * toSeconds(m_temporalLength) + 1;  // 60 FPS

//     // Find all the positions the Viper segments will move through
//     m_track.clear();
//     for (int i = 0; i < numberOfPoints; ++i) {
//         m_track.create_back(
//             headPosition - dL * i,
//             headTime - (m_temporalLength * i) / (numberOfPoints - 1));
//     }

//     m_headPoint = m_track.front();
// }

void Viper::setup(const Vec2& headPosition, double angle,
                  const Time& viperTemporalLength) {
    m_angle = angle;
    //Vec2 vipVec = Vec2(cos(degToRad(angle)), sin(degToRad(angle)));
    m_temporalLength = seconds(0);
    growth(viperTemporalLength);
    m_track.create_back( headPosition, seconds(0));
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
    if (!afterTail) {
        m_track.clear();
        if (m_state & ViperAlive)
            throw std::runtime_error(
                "About to remove all TrackPoints, this cannot be right.");
        return;
    }
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
    if (m_state == ViperDead) {
        DestroyMeEvent event(this);
        notify(&event);
        return;
    }
    if (m_state == ViperDying)
        die(elapsedTime);  // Allow the viper to die for a while
    else {
        m_headPoint = createNextHeadTrackPoint(elapsedTime);
        grow(elapsedTime);
    }
    updateBodies();
    cleanUpTrailingTrackPoints();
}

void Viper::updateBodies() {
    Time headLength = std::min(m_temporalLength, s_headTemporalLength);
    Time tailLength =
        std::min(m_temporalLength - headLength, s_tailTemporalLength);
    Time bodyLength = m_temporalLength - headLength - tailLength;

    Time headFront = m_headPoint->getTime();
    Time bodyFront = headFront - headLength;
    Time tailFront = bodyFront - bodyLength;

    const size_t nHeadNodes = ViperSketch::headNodes().size();
    const size_t nBodyNodes = ViperSketch::bodyNodes().size();
    const size_t nTailNodes = ViperSketch::tailNodes().size();

    // HEAD
    updateBody(m_headBody, headFront, headLength);
    // BODY
    updateBody(m_bodyBody, bodyFront, bodyLength);
    // TAIL
    updateBody(m_tailBody, tailFront, tailLength);
}

void Viper::updateBody(CollisionVertices& body, const Time& timeFront,
                       const Time& temporalLength) {
    body.clear();
    if (temporalLength <= seconds(0))
        return;
    double width = 20;  // TODO:Adapt width depending on how streched the
                        // segment is, i.e., dL/dt
    const std::vector<Vec2>* sketch;
    Vec2 textureSize;
    int numberOfSegments = 1;
    Time segmentLength = temporalLength;
    switch (body.partID) {
        case ViperHead: {
            sketch = &ViperSketch::headNodes();
            textureSize = m_headTexture.getSize();
            break;
        }
        case ViperBody: {
            sketch = &ViperSketch::bodyNodes();
            textureSize = m_bodyTexture.getSize();
            segmentLength = s_bodyTemporalLength;
            // Rounded upwards to allow for a fraction of a full segment
            numberOfSegments = temporalLength / segmentLength + 1;
            break;
        }
        case ViperTail: {
            sketch = &ViperSketch::tailNodes();
            textureSize = m_tailTexture.getSize();
            break;
        }
    }
    // Used to extend a segment when, e.g., growing
    Time leftOverLength = temporalLength - segmentLength * numberOfSegments;
    int sketchIndex = 0;
    for (int segmentIndex = 0; segmentIndex < numberOfSegments;
         ++segmentIndex) {
        while (sketchIndex < sketch->size()) {
            Time L = (body.partID == ViperBody and
                      (segmentIndex == numberOfSegments - 1))
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
            body.appendVertex(sf::Vertex(position, m_mainColor, textCoords));
            ++sketchIndex;
        }
        sketchIndex = 2;
    }

    // Prepare CollisionParts
    switch (body.partID) {
        case ViperHead: {
            body.assignBodyParts(0, 4, 4, ViperHead | ViperSensitivePart, 0,
                                 true);
            body.assignBodyParts(2, body.size() - 2, 4, body.partID, 2);
            break;
        }
        case ViperBody:
        case ViperTail: {
            body.assignBodyParts(0, body.size(), 4, body.partID, 2);
            break;
        }
    }
    infoTag();
}

}  // namespace VVipers