#include <SFML/Graphics/RenderTarget.hpp>
#include <exception>
#include <vvipers/GameElements/Viper.hpp>
#include <vvipers/Utilities/VVColor.hpp>
#include <vvipers/Utilities/Vec2.hpp>
#include <vvipers/Utilities/debug.hpp>
#include <vvipers/config.hpp>

namespace VVipers {

using namespace std::chrono_literals;

/** Nested helper class. Viper class holds one static instance. **/
class Viper::ViperConfiguration {
    friend class Viper;

  private:
    void initialize(const OptionsProvider& options,
                    const TextureProvider& textures) {
        nominalSpeed = options.getOptionDouble("Viper/nominalSpeed");
        nominalSegmentWidth =
            options.getOptionDouble("Viper/nominalSegmentWidth");  // px
        boostMaxCharge =
            seconds(options.getOptionDouble("Viper/boostMaxCharge"));  // s
        boostRechargeRate =
            options.getOptionDouble("Viper/boostRechargeRate");  // s per s
        boostRechargeCooldown = seconds(options.getOptionDouble(
            "Viper/boostRechargeCooldown"));  // Countdown start

        headNominalLength = options.getOptionDouble(
            "ViperModel/ViperHead/nominalLength");                 // px
        headDuration = seconds(headNominalLength / nominalSpeed);  // s
        headNodes =
            options.getOption2DVectorArray("ViperModel/ViperHead/nodes");

        bodyNominalLength = options.getOptionDouble(
            "ViperModel/ViperBody/nominalLength");                 // px
        bodyDuration = seconds(bodyNominalLength / nominalSpeed);  // s
        bodyNodes =
            options.getOption2DVectorArray("ViperModel/ViperBody/nodes");

        tailNominalLength = options.getOptionDouble(
            "ViperModel/ViperTail/nominalLength");                 // px
        tailDuration = seconds(tailNominalLength / nominalSpeed);  // s
        tailNodes =
            options.getOption2DVectorArray("ViperModel/ViperTail/nodes");

        headTexture = textures.getTexture("ViperHead");
        bodyTexture = textures.getTexture("ViperBody");
        tailTexture = textures.getTexture("ViperTail");

        // Find for which index the head is the widest
        lastActiveIndex = 0;
        double wMax = 0;
        for (size_t i = 0; i < headNodes.size(); ++i) {
            if (std::abs(headNodes[i].x) >= wMax) {
                lastActiveIndex = i;
                wMax = std::abs(headNodes[i].x);
            }
        }

        initialized = true;
    }

  public:
    bool initialized = false;
    double nominalSpeed;         // px/s
    double nominalSegmentWidth;  // px
    Time boostMaxCharge;         // s
    double boostRechargeRate;    // s per s
    Time boostRechargeCooldown;  // Countdown start

    size_t lastActiveIndex;

    double headNominalLength;  // px
    Time headDuration;         // s
    std::vector<Vec2> headNodes;
    const sf::Texture* headTexture;

    double bodyNominalLength;  // px
    Time bodyDuration;         // s
    std::vector<Vec2> bodyNodes;
    const sf::Texture* bodyTexture;

    double tailNominalLength;  // px
    Time tailDuration;         // s
    std::vector<Vec2> tailNodes;
    const sf::Texture* tailTexture;
};

Viper::ViperConfiguration Viper::viperCfg;

Viper::Viper(const OptionsProvider& options, const TextureProvider& textures)
    : ColliderSegmented(true),
      m_boostInc(0.),
      m_boostRechargeCooldown(0.),
      m_growth(0.),
      m_headPoint(nullptr) {
    if (!viperCfg.initialized)
        viperCfg.initialize(options, textures);
    m_boostCharge = viperCfg.boostMaxCharge;

    m_nominalSpeed = viperCfg.nominalSpeed;
    m_speed = m_nominalSpeed;
}

void Viper::eat(const Food& food) {
    addGrowth(viperCfg.bodyDuration * (food.getRadius() * food.getRadius()) /
                  (Food::nominalFoodRadius * Food::nominalFoodRadius),
              m_headPoint->getTime());
    addBoostCharge(0.5s);
}

void Viper::die(const Time& elapsedTime) {
    m_temporalLength -= 4 * elapsedTime;
    if (m_temporalLength <= seconds(0))
        state(Dead);
}

void Viper::addGrowth(Time howMuch, Time when) {
    m_dinnerTimes[when] = howMuch;
}

double Viper::getLength() const {
    return m_track.length(m_headPoint->getTime(),
                          m_headPoint->getTime() - m_temporalLength);
}

double Viper::getNominalWidth() const { return viperCfg.nominalSegmentWidth; }

void Viper::setup(const Vec2& tailPosition, double angle,
                  double numberOfBodySegments) {
    m_angle = angle;
    m_temporalLength = viperCfg.headDuration + viperCfg.tailDuration;
    m_growth = numberOfBodySegments * viperCfg.bodyDuration;

    Vec2 direction = Vec2(1, 0).rotate(angle);
    double length = toSeconds(m_temporalLength) * getSpeed();
    auto viperVector = length * direction;
    m_track.create_back(tailPosition + viperVector, m_temporalLength);
    m_track.create_back(tailPosition, seconds(0));
    m_headPoint = m_track.front();
}

TrackPoint* Viper::createNextHeadTrackPoint(Time elapsedTime) {
    Vec2 advance = Vec2(m_speed * toSeconds(elapsedTime), 0).rotate(m_angle);
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

void Viper::clearDinnerTimes() {
    Time tailTime = m_headPoint->getTime() - m_temporalLength;
    // Slightly wasteful but there will never be more than a handful dinner
    // times stored at the same time.
    for (auto& dinnerTime : m_dinnerTimes)
        if (dinnerTime.first < tailTime) {
            m_growth += dinnerTime.second;
            dinnerTime.second = seconds(0);
        }
    // This loop can only remove a maximum of one time per update. But that's
    // fine
    for (auto& dinnerTime : m_dinnerTimes)
        // Times 10 give us some margin but it would be nicer to specify
        // exactly...
        if (dinnerTime.first + 10 * viperCfg.bodyDuration < tailTime) {
            m_dinnerTimes.erase(dinnerTime.first);
            break;
        }
}

Time Viper::getBoostMax() const { return viperCfg.boostMaxCharge; }

void Viper::grow(const Time& elapsedTime) {
    // Limit the growth to how much time that has passed
    Time actualGrowth = std::min(m_growth, elapsedTime);
    m_temporalLength += actualGrowth;
    m_growth -= actualGrowth;
}

// This allows the viper narrower turns when not boosting
double Viper::getMaxAngularSpeed() const {
    return m_nominalSpeed / viperCfg.nominalSegmentWidth;
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
    updateVertices();
    cleanUpTrailingTrackPoints();
    clearDinnerTimes();
}

void Viper::addBoostCharge(Time charge) {
    auto oldCharge = m_boostCharge;
    m_boostCharge += charge;
    m_boostCharge = std::max(m_boostCharge, seconds(0));
    m_boostCharge = std::min(m_boostCharge, getBoostMax());

    if (m_boostCharge != oldCharge) {
        ObjectModifiedEvent event(this);
        notify(&event);
    }
}

void Viper::updateBoostCharge(Time elapsedTime) {
    if (m_boostRechargeCooldown == seconds(0)) {
        addBoostCharge(elapsedTime * viperCfg.boostRechargeRate);
    } else if (m_boostInc > 0)
        addBoostCharge(-elapsedTime);
}

void Viper::updateSpeed(const Time& elapsedTime) {
    double acceleration = 0;
    double targetSpeed = viperCfg.nominalSpeed;
    if (m_boostInc > 0) {
        targetSpeed *= (1 + m_boostInc);
        m_boostRechargeCooldown = viperCfg.boostRechargeCooldown;
    } else {
        m_boostRechargeCooldown -=
            std::min(m_boostRechargeCooldown, elapsedTime);
    }
    updateBoostCharge(elapsedTime);
    if (m_speed < targetSpeed) {
        // 0.5s to increase speed by nominal speed but cap at targetSpeed
        acceleration =
            std::min(2 * viperCfg.nominalSpeed,
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

void Viper::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = viperCfg.headTexture;
    target.draw(&m_verticesHead[0], m_verticesHead.size(),
                sf::PrimitiveType::TriangleStrip, states);
    states.texture = viperCfg.bodyTexture;
    target.draw(&m_verticesBody[0], m_verticesBody.size(),
                sf::PrimitiveType::TriangleStrip, states);
    states.texture = viperCfg.tailTexture;
    target.draw(&m_verticesTail[0], m_verticesTail.size(),
                sf::PrimitiveType::TriangleStrip, states);
}

bool Viper::isSegmentActive(size_t i) const {
    return 2 * i < viperCfg.lastActiveIndex;
}

size_t Viper::getSegmentCount() const {
    int nPoints = m_verticesHead.size();
    if (m_verticesBody.size() > 0)
        nPoints += m_verticesBody.size() - 2;
    if (m_verticesTail.size() > 0)
        nPoints += m_verticesTail.size() - 2;
    return std::max(0, nPoints / 2 - 1);
}

Vec2 Viper::getSegmentGlobalPoint(size_t segmentIndex,
                                  size_t pointIndex) const {
    // Due to the need for vertex coordinates there are overlaping positions
    // in the vertex array. These are not relevant for collision detection
    // so we skip them
    if (2 * segmentIndex + 2 < m_verticesHead.size()) {
        return m_verticesHead[2 * segmentIndex + pointIndex].position;
    } else {
        segmentIndex -= (m_verticesHead.size() - 2) / 2;
    }
    if (2 * segmentIndex + 2 < m_verticesBody.size()) {
        return m_verticesBody[2 * segmentIndex + pointIndex].position;
    } else {
        segmentIndex -= (m_verticesBody.size() - 2) / 2;
    }
    return m_verticesTail[2 * segmentIndex + pointIndex].position;
}

void Viper::updateVertices() {
    Time headDuration = std::min(m_temporalLength, viperCfg.headDuration);
    if (m_temporalLength < viperCfg.headDuration + viperCfg.tailDuration) {
        headDuration = m_temporalLength *
                       (viperCfg.headDuration /
                        (viperCfg.headDuration + viperCfg.tailDuration));
    }
    Time tailDuration =
        std::min(m_temporalLength - headDuration, viperCfg.tailDuration);
    Time bodyDuration = m_temporalLength - headDuration - tailDuration;

    Time headFront = m_headPoint->getTime();
    Time bodyFront = headFront - headDuration;
    Time tailFront = bodyFront - bodyDuration;

    // HEAD
    m_verticesHead.clear();
    updateVertices(ViperPart::Head, headFront, headDuration);
    // BODY
    m_verticesBody.clear();
    updateVertices(ViperPart::Body, bodyFront, bodyDuration);
    // TAIL
    m_verticesTail.clear();
    updateVertices(ViperPart::Tail, tailFront, tailDuration);
}

// I hate this code. It needs to be divided into smaller manageble pieces.
void Viper::updateVertices(ViperPart part, Time timeFront,
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

    // True width is calculated later and proportional to dL/dt
    double nominalWidth = viperCfg.nominalSegmentWidth;
    // Determining model, texture and lengths depending on the type of segments
    const std::vector<Vec2>* sketch;
    std::vector<sf::Vertex>* vertices;
    Vec2 textureSize;
    int numberOfSegments = 1;
    Time segmentLength = temporalLength;
    switch (part) {
        case ViperPart::Head: {
            sketch = &viperCfg.headNodes;
            vertices = &m_verticesHead;
            textureSize = viperCfg.headTexture->getSize();
            break;
        }
        case ViperPart::Body: {
            sketch = &viperCfg.bodyNodes;
            vertices = &m_verticesBody;
            textureSize = viperCfg.bodyTexture->getSize();
            const Time nominalSegmentDuration = viperCfg.bodyDuration;
            segmentLength = std::min(temporalLength, nominalSegmentDuration);
            // The growing segment starts from 0 and has a fraction of the
            // nominal segment length
            numberOfSegments = temporalLength / nominalSegmentDuration + 1;
            break;
        }
        case ViperPart::Tail: {
            sketch = &viperCfg.tailNodes;
            vertices = &m_verticesTail;
            textureSize = viperCfg.tailTexture->getSize();
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
        for (size_t iNode = (iSeg == 0) ? 0 : 2; iNode < sketch->size();
             ++iNode) {
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
            sf::Color color = calcVertexColor(time);
            vertices->push_back({position, color, textCoords});
        }
        timeFront -= actualLength;  // Next segment will start here
    }
}

sf::Color Viper::calcVertexColor(Time time) {
    auto headSide = m_dinnerTimes.lower_bound(time);  // Closer to head
    auto tailSide = m_dinnerTimes.end();
    if (headSide != m_dinnerTimes.begin()) {
        tailSide = headSide;
        --tailSide;  // Closer to tail
    }

    auto distanceLimit = 2 * viperCfg.bodyDuration;
    double sFactor = 0;
    if (tailSide != m_dinnerTimes.end()) {
        auto distance = time - tailSide->first;
        if (distance < distanceLimit)
            sFactor += (distanceLimit - distance) / (distanceLimit);
    }
    if (headSide != m_dinnerTimes.end()) {
        auto distance = headSide->first - time;
        if (distance < distanceLimit)
            sFactor += (distanceLimit - distance) / (distanceLimit);
    }
    sFactor = std::min(1.0, sFactor);
    return blendColors(m_primaryColor, 1. - sFactor, m_secondaryColor, sFactor);
}

}  // namespace VVipers