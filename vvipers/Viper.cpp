#include <SFML/Graphics/RenderTarget.hpp>
#include <exception>
#include <vvipers/Vec2.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/ViperConfig.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;

Viper::Viper()
    : m_speed(ViperConfig::properties().nominalSpeed),
      m_boostInc(0.),
      m_boostCharge(0.),
      m_growth(0.),
      m_headPoint(nullptr),
      m_color(sf::Color::Green) {
    loadTextures();
}

void Viper::boost(double relativeSpeedIncrease) {
    m_boostInc = relativeSpeedIncrease;
}

void Viper::die(const Time& elapsedTime) {
    m_temporalLength -= 3*elapsedTime;
    if (m_temporalLength <= seconds(0))
        state(Dead);
}

void Viper::addGrowth(const Time& g) { m_growth += g; }

double Viper::length() const {
    return m_track.length(m_headPoint->getTime(),
                          m_headPoint->getTime() - m_temporalLength);
}

void Viper::setup(const Vec2& headPosition, double angle,
                  double numberOfBodySegments) {
    m_angle = angle;
    // Vec2 vipVec = Vec2(cos(degToRad(angle)), sin(degToRad(angle)));
    m_temporalLength = seconds(0);
    addGrowth(ViperConfig::properties().headDuration +
              numberOfBodySegments * ViperConfig::properties().bodyDuration +
              ViperConfig::properties().tailDuration);
    m_track.create_back(headPosition, seconds(0));
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
        if (state() == Alive or state() == Dying )
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

bool Viper::isSensitive(const Bodypart* bp) const {
    return std::find(m_sensitiveParts.begin(), m_sensitiveParts.end(), bp) !=
           m_sensitiveParts.end();
}

void Viper::loadTextures() {
    // Load image and split it into the different textures.
    sf::Image combinedTextureImage;
    combinedTextureImage.loadFromFile(VIPER_TEXTURE_FILE_PATH);
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

void Viper::update(Time elapsedTime) {
    if (state() == Dead) {
        DestroyEvent event(this);
        notify(&event);
        return;
    }
    if (state() == Dying)
        die(elapsedTime);  // Allow the viper to die for a while
    else {
        m_headPoint = createNextHeadTrackPoint(elapsedTime);
        grow(elapsedTime);
    }
    updateMotion(elapsedTime);
    updateBodies();
    cleanUpTrailingTrackPoints();
}

void Viper::updateSpeed(const Time& elapsedTime) {
    double acceleration = 0;
    double targetSpeed =
        (1 + m_boostInc) * ViperConfig::properties().nominalSpeed;
    if (m_boostInc > 0) {
        m_boostCharge -= std::min(m_boostCharge, elapsedTime);
        m_boostRechargeCooldown =
            ViperConfig::properties().boostRechargeCooldown;
    } else {
        m_boostRechargeCooldown -=
            std::min(m_boostRechargeCooldown, elapsedTime);
    }
    if (m_boostRechargeCooldown == seconds(0)) {
        m_boostCharge = std::min(
            m_boostCharge +
                elapsedTime * ViperConfig::properties().boostRechargeRate,
            ViperConfig::properties().boostMaxCharge);
    }
    if (m_speed < targetSpeed) {
        // 1s to increase speed by nominal speed but cap at targetSpeed
        acceleration =
            std::min(ViperConfig::properties().nominalSpeed,
                     (targetSpeed - m_speed) / toSeconds(elapsedTime));
    } else if (m_speed > targetSpeed) {
        acceleration =
            std::max(-ViperConfig::properties().nominalSpeed,
                     (targetSpeed - m_speed) / toSeconds(elapsedTime));
    }
    m_speed += acceleration * toSeconds(elapsedTime);
}

void Viper::updateAngle(const Time& elapsedTime) {
    m_angle += m_angularSpeed * toSeconds(elapsedTime);
}

void Viper::updateMotion(const Time& elapsedTime) {
    updateSpeed(elapsedTime);
    updateAngle(elapsedTime);
}

void Viper::updateBodies() {
    Time headDuration =
        std::min(m_temporalLength, ViperConfig::properties().headDuration);
    Time tailDuration = std::min(m_temporalLength - headDuration,
                                 ViperConfig::properties().tailDuration);
    Time bodyDuration = m_temporalLength - headDuration - tailDuration;

    Time headFront = m_headPoint->getTime();
    Time bodyFront = headFront - headDuration;
    Time tailFront = bodyFront - bodyDuration;

    const size_t nHeadNodes = ViperConfig::properties().headNodes.size();
    const size_t nBodyNodes = ViperConfig::properties().bodyNodes.size();
    const size_t nTailNodes = ViperConfig::properties().tailNodes.size();

    // HEAD
    m_headBody.clear();
    updateBody(ViperPart::Head, headFront, headDuration);
    // BODY
    m_bodyBody.clear();
    updateBody(ViperPart::Body, bodyFront, bodyDuration);
    // TAIL
    m_tailBody.clear();
    updateBody(ViperPart::Tail, tailFront, tailDuration);
}

// Helper function for updateBody
int findMaxWidthIndex(const std::vector<Vec2>& v) {
    int iMax = 0;
    double wMax = 0;
    for (int i = 0; i < v.size(); ++i) {
        if (std::abs(v[i].x) >= wMax) {
            iMax = i;
            wMax = std::abs(v[i].x);
        }
    }
    return iMax;
}

// I hate this code. It needs to be divided into smaller manageble pieces.
void Viper::updateBody(ViperPart part, Time timeFront,
                       const Time& temporalLength) {
    /* This code does the following in order to calculate the vertices for one
     * parts of the viper. It does mainly the same thing but not exactly
     * depending on which kind of parts it is. Since there might be several body
     * segments the exceptions are mostly here.
     * 1) Checks what kind of snake
     *    segment we are working with and sets a few variables based on this.
     * 2) Calculates the position and texture coordinates for all segments (only
     *    one for head and tail).
     *       2a) If making several segments the two first vertices are skipped
     *           over for all segments except the first one.
     *      2b) Any left over temporal length is put in a slightly smaller body
     *          segment in the end.
     * 3) Assigns bodyparts to be used by the collision system. Only the first
     *    parts of the head are "active".
     */
    if (temporalLength <= seconds(0))
        return;

    CollisionVertices* body;
    // TODO:Adapt width depending on how streched the segment is, i.e., dL/dt
    double width = ViperConfig::properties().nominalSegmentWidth;
    // Determining model, texture and lengths depending on the type of segments
    const std::vector<Vec2>* sketch;
    Vec2 textureSize;
    int numberOfSegments = 1;
    Time segmentLength = temporalLength;
    switch (part) {
        case ViperPart::Head: {
            body = &m_headBody;
            sketch = &ViperConfig::properties().headNodes;
            textureSize = m_headTexture.getSize();
            break;
        }
        case ViperPart::Body: {
            body = &m_bodyBody;
            sketch = &ViperConfig::properties().bodyNodes;
            textureSize = m_bodyTexture.getSize();
            const Time nominalSegmentDuration =
                ViperConfig::properties().bodyDuration;
            segmentLength = std::min(temporalLength, nominalSegmentDuration);
            // OPTION 1: The growing segment starts from 0 and has a fraction of
            // the nominal segment length
            numberOfSegments = temporalLength / nominalSegmentDuration + 1;
            // OPTION 2: Unless it's the only segment the last segment grows
            // until it splits in half
            break;
        }
        case ViperPart::Tail: {
            body = &m_tailBody;
            sketch = &ViperConfig::properties().tailNodes;
            textureSize = m_tailTexture.getSize();
            break;
        }
    }

    // Calc position and texture coordinates
    for (int iSeg = 0; iSeg < numberOfSegments; ++iSeg) {
        Time actualLength = segmentLength;
        // If it is the last segment: use up whatever is left instead
        if (iSeg == numberOfSegments - 1)
            actualLength =
                temporalLength - segmentLength * (numberOfSegments - 1);
        // Start on 0 if it is the first segment, otherwise on 2
        for (int iNode = (iSeg == 0) ? 0 : 2; iNode < sketch->size(); ++iNode) {
            // How far we have come so far.
            Time time = timeFront - sketch->at(iNode).y * actualLength;
            // This is the position at the snake axis
            Vec2 mid = m_track.position(time);
            // This vector reaches out the the side of the snake
            Vec2 perp = m_track.direction(time).perpVec() *
                        (width * sketch->at(iNode).x);
            // Resulting position
            Vec2 position = mid + perp;
            // Use the whole width of the texture and repeat the texture if
            // there are several segments
            Vec2 textCoords =
                (Vec2(0.5f, iSeg) + sketch->at(iNode)) * textureSize;
            // All vertices have the same color atm
            body->appendVertex(sf::Vertex(position, m_color, textCoords));
        }
        timeFront -= actualLength;  // Next segment will start here
    }

    // Prepare bodyparts
    const int sharedNodes = 2;
    const int nodesPerBodypart = 4;

    if (part == ViperPart::Head) {
        // Looks for the widest part. If two parts have the same width we
        // want the last one.
        const int iMax = findMaxWidthIndex(*sketch);
        int sensitiveVertices = iMax + 1;  // This must be >= 4
        body->assignBodyParts(0, sensitiveVertices, nodesPerBodypart,
                              sharedNodes, true);
        body->assignBodyParts(sensitiveVertices - 2,
                              body->size() - sensitiveVertices,
                              nodesPerBodypart, sharedNodes);

        const int sensitiveParts = (sensitiveVertices + 2) / 4;
        m_sensitiveParts.resize(sensitiveParts);
        for (int i = 0; i < sensitiveParts; ++i)
            m_sensitiveParts[i] = body->bodyparts()[i];
    } else
        body->assignBodyParts(0, body->size(), nodesPerBodypart, sharedNodes);
}

}  // namespace VVipers