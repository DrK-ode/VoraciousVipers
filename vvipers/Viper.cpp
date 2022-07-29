#include <SFML/Graphics/RenderTarget.hpp>
#include <exception>
#include <vvipers/GameOptions.hpp>
#include <vvipers/Vec2.hpp>
#include <vvipers/Viper.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;

/** Nested helper class. Viper class holds one static instance. **/
class Viper::ViperConfiguration {
    friend class Viper;

  private:
    void initialize() {
        nominalSpeed = GameOptions::getOptionDouble("Viper/nominalSpeed");
        nominalSegmentWidth =
            GameOptions::getOptionDouble("Viper/nominalSegmentWidth");  // px
        boostMaxCharge =
            seconds(GameOptions::getOptionDouble("Viper/boostMaxCharge"));  // s
        boostRechargeRate =
            GameOptions::getOptionDouble("Viper/boostRechargeRate");  // s per s
        boostRechargeCooldown = seconds(GameOptions::getOptionDouble(
            "Viper/boostRechargeCooldown"));  // Countdown start

        headNominalLength = GameOptions::getOptionDouble(
            "ViperModel/ViperHead/nominalLength");                 // px
        headDuration = seconds(headNominalLength / nominalSpeed);  // s
        headNodes =
            GameOptions::getOption2DVectorArray("ViperModel/ViperHead/nodes");

        bodyNominalLength = GameOptions::getOptionDouble(
            "ViperModel/ViperBody/nominalLength");                 // px
        bodyDuration = seconds(bodyNominalLength / nominalSpeed);  // s
        bodyNodes =
            GameOptions::getOption2DVectorArray("ViperModel/ViperBody/nodes");

        tailNominalLength = GameOptions::getOptionDouble(
            "ViperModel/ViperTail/nominalLength");                 // px
        tailDuration = seconds(tailNominalLength / nominalSpeed);  // s
        tailNodes =
            GameOptions::getOption2DVectorArray("ViperModel/ViperTail/nodes");

        loadTextures();

        initialized = true;
    }
    void loadTextures() {
        // Load image and split it into the different textures.
        sf::Image combinedTextureImage;
        std::stringstream ss;
        ss << GameOptions::getOptionString("General/resourceDirectoryPath")
           << GameOptions::getOptionString("ViperModel/textureFileName");
        combinedTextureImage.loadFromFile(ss.str());
        sf::Vector2i imgSize(combinedTextureImage.getSize());
        sf::Vector2i upperLeft;
        sf::Vector2i rectSize(imgSize.x, imgSize.y / 3);

        // Head texture
        upperLeft = {0, 0};
        headTexture.loadFromImage(combinedTextureImage,
                                  sf::IntRect(upperLeft, rectSize));
        // Body texture
        upperLeft.y = rectSize.y;
        bodyTexture.loadFromImage(combinedTextureImage,
                                  sf::IntRect(upperLeft, rectSize));
        bodyTexture.setRepeated(true);
        // Tail texture
        upperLeft.y = 2 * rectSize.y;
        tailTexture.loadFromImage(combinedTextureImage,
                                  sf::IntRect(upperLeft, rectSize));
    }

  public:
    bool initialized = false;
    double nominalSpeed;         // px/s
    double nominalSegmentWidth;  // px
    Time boostMaxCharge;         // s
    double boostRechargeRate;    // s per s
    Time boostRechargeCooldown;  // Countdown start

    double headNominalLength;  // px
    Time headDuration;         // s
    std::vector<Vec2> headNodes;
    sf::Texture headTexture;

    double bodyNominalLength;  // px
    Time bodyDuration;         // s
    std::vector<Vec2> bodyNodes;
    sf::Texture bodyTexture;

    double tailNominalLength;  // px
    Time tailDuration;         // s
    std::vector<Vec2> tailNodes;
    sf::Texture tailTexture;
};

Viper::ViperConfiguration Viper::viperCfg;

Viper::Viper()
    : m_boostInc(0.),
      m_boostCharge(0.),
      m_growth(0.),
      m_headPoint(nullptr),
      m_color(sf::Color::Green) {
    if (!viperCfg.initialized)
        viperCfg.initialize();

    m_speed = viperCfg.nominalSpeed;
    m_headBody.setTexture(&viperCfg.headTexture);
    m_bodyBody.setTexture(&viperCfg.bodyTexture);
    m_tailBody.setTexture(&viperCfg.tailTexture);
}

void Viper::boost(double relativeSpeedIncrease) {
    m_boostInc = relativeSpeedIncrease;
}

void Viper::die(const Time& elapsedTime) {
    m_temporalLength -= 3 * elapsedTime;
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
    addGrowth(viperCfg.headDuration +
              numberOfBodySegments * viperCfg.bodyDuration +
              viperCfg.tailDuration);
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
        if (state() == Alive or state() == Dying)
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

Time Viper::boostMax() const { return viperCfg.boostMaxCharge; }

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

double Viper::turningRadius() const {
    return viperCfg.nominalSpeed / viperCfg.nominalSegmentWidth;
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

void Viper::updateBoostCharge(Time chargeChange) {
    chargeChange = std::max(chargeChange, -m_boostCharge);
    chargeChange =
        std::min(chargeChange, viperCfg.boostMaxCharge - m_boostCharge);

    m_boostCharge += chargeChange;
    BoostEvent event(chargeChange, m_boostCharge, viperCfg.boostMaxCharge);
    notify(&event);
}

void Viper::updateSpeed(const Time& elapsedTime) {
    double acceleration = 0;
    double targetSpeed = viperCfg.nominalSpeed;
    if (m_boostInc > 0) {
        targetSpeed *= (1 + m_boostInc);
        updateBoostCharge(-elapsedTime);
        m_boostRechargeCooldown = viperCfg.boostRechargeCooldown;
    } else {
        m_boostRechargeCooldown -=
            std::min(m_boostRechargeCooldown, elapsedTime);
    }
    if (m_boostRechargeCooldown == seconds(0) and
        m_boostCharge < viperCfg.boostMaxCharge) {
        updateBoostCharge(elapsedTime * viperCfg.boostRechargeRate);
    }
    if (m_speed < targetSpeed) {
        // 1s to increase speed by nominal speed but cap at targetSpeed
        acceleration =
            std::min(viperCfg.nominalSpeed,
                     (targetSpeed - m_speed) / toSeconds(elapsedTime));
    } else if (m_speed > targetSpeed) {
        acceleration =
            std::max(-viperCfg.nominalSpeed,
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
    Time headDuration = std::min(m_temporalLength, viperCfg.headDuration);
    Time tailDuration =
        std::min(m_temporalLength - headDuration, viperCfg.tailDuration);
    Time bodyDuration = m_temporalLength - headDuration - tailDuration;

    Time headFront = m_headPoint->getTime();
    Time bodyFront = headFront - headDuration;
    Time tailFront = bodyFront - bodyDuration;

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
    for (size_t i = 0; i < v.size(); ++i) {
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
    // True width is calculated later and proportional to dL/dt
    double nominalWidth = viperCfg.nominalSegmentWidth;
    // Determining model, texture and lengths depending on the type of segments
    const std::vector<Vec2>* sketch;
    Vec2 textureSize;
    int numberOfSegments = 1;
    Time segmentLength = temporalLength;
    switch (part) {
        case ViperPart::Head: {
            body = &m_headBody;
            sketch = &viperCfg.headNodes;
            textureSize = viperCfg.headTexture.getSize();
            break;
        }
        case ViperPart::Body: {
            body = &m_bodyBody;
            sketch = &viperCfg.bodyNodes;
            textureSize = viperCfg.bodyTexture.getSize();
            const Time nominalSegmentDuration = viperCfg.bodyDuration;
            segmentLength = std::min(temporalLength, nominalSegmentDuration);
            // The growing segment starts from 0 and has a fraction of the
            // nominal segment length
            numberOfSegments = temporalLength / nominalSegmentDuration + 1;
            break;
        }
        case ViperPart::Tail: {
            body = &m_tailBody;
            sketch = &viperCfg.tailNodes;
            textureSize = viperCfg.tailTexture.getSize();
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
        for (size_t iNode = (iSeg == 0) ? 0 : 2; iNode < sketch->size(); ++iNode) {
            // How far we have come so far.
            Time time = timeFront - sketch->at(iNode).y * actualLength;
            // This is the position at the snake axis
            Vec2 mid = m_track.position(time);
            // This vector reaches out the the side of the snake
            Vec2 gradient = m_track.gradient(time);
            double width =
                nominalWidth * viperCfg.nominalSpeed / gradient.abs();
            Vec2 perp =
                gradient.normalized().perpVec() * (width * sketch->at(iNode).x);
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