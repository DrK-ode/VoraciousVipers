#include <exception>
#include <vvipers/Vec2.hpp>
#include <vvipers/ViperPhysics.hpp>
#include <vvipers/ViperSketch.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

using namespace std::chrono_literals;
const Time ViperPhysics::s_headTemporalLength(1s);
const Time ViperPhysics::s_bodyTemporalLength(1s);
const Time ViperPhysics::s_tailTemporalLength(1s);

const double ViperPhysics::s_nominalSpeed(60.);

ViperPhysics::ViperPhysics()
    : m_acc(0.), m_speed(s_nominalSpeed), m_growth(0), m_head(nullptr) {}

void ViperPhysics::growth(const Time& g) { m_growth += g; }

double ViperPhysics::length() const {
    return m_track.length(m_head->getTime(),
                          m_head->getTime() - m_temporalLength);
}

// Each initial segment will get the same length and be setup in a line from the
// start coordinates to the end coordinates. The track will be prepared assuming
// nominal speed. Tail at from-vector, head at to-vector.
void ViperPhysics::setup(const Vec2& headPosition, double angle,
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
    logInfo("Filling track with ", numberOfPoints, " track points.");
    for (int i = 0; i < numberOfPoints; ++i) {
        m_track.create_back(
            headPosition - dL * i,
            headTime - (m_temporalLength * i) / (numberOfPoints - 1));
    }

    m_head = m_track.front();
}

TrackPoint* ViperPhysics::createNextHeadTrackPoint(Time elapsedTime) {
    double dx = m_speed * toSeconds(elapsedTime) * cos(degToRad(m_angle));
    double dy = m_speed * toSeconds(elapsedTime) * sin(degToRad(m_angle));
    Vec2 advance(dx, dy);
    return m_track.create_front(*m_head + advance,
                                m_head->getTime() + elapsedTime);
}

void ViperPhysics::cleanUpTrailingTrackPoints() {
    // Removes any point(s) the whole Viper has passed through
    Time tailTime = m_head->getTime() - m_temporalLength;
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

void ViperPhysics::grow(const Time& elapsedTime) {
    // Limit the growth to how much time that has passed
    Time actualGrowth = std::min(m_growth, elapsedTime);
    m_temporalLength += actualGrowth;
    m_growth -= actualGrowth;
}

void ViperPhysics::update(const Time& elapsedTime) {
    m_head = createNextHeadTrackPoint(elapsedTime);
    grow(elapsedTime);
    updateCollidable();
    cleanUpTrailingTrackPoints();
}

void ViperPhysics::updateNodes() {
    if (m_temporalLength < s_headTemporalLength + s_tailTemporalLength)
        throw std::runtime_error("Viper is too small");

    Time headLength = s_headTemporalLength;
    size_t numberOfBodySegments = std::max(
        0., (m_temporalLength - headLength) / s_bodyTemporalLength - 1.);
    Time bodyLength = s_bodyTemporalLength * numberOfBodySegments;
    Time tailLength = m_temporalLength - headLength - bodyLength;

    Time headFront = m_head->getTime();
    Time bodyFront = headFront - headLength;
    Time tailFront = bodyFront - bodyLength;

    m_collidableParts.resize(numberOfBodySegments + 2);
    // HEAD
    CollidablePart& head = m_collidableParts.front();
    head.label = "Head";
    updateNodes(headFront, headLength, ViperSketch::headNodes(), head.nodes);
    // BODY
    if (numberOfBodySegments > 0) {
        const Time segmentLength = bodyLength / numberOfBodySegments;
        for (int i = 0; i < numberOfBodySegments; ++i) {
            CollidablePart& bodyPart = m_collidableParts[i + 1];
            bodyPart.label = "Body";
            updateNodes(bodyFront - segmentLength * i, segmentLength,
                        ViperSketch::bodyNodes(), bodyPart.nodes);
        }
    }
    // TAIL
    CollidablePart& tail = m_collidableParts.back();
    tail.label = "Tail";
    updateNodes(tailFront, tailLength, ViperSketch::tailNodes(), tail.nodes);
}

// Helper function since the prepare methods share most of the code
void ViperPhysics::updateNodes(const Time& timeFront,
                               const Time& temporalLength,
                               const std::vector<Vec2>& relativePosition,
                               std::vector<Vec2>& nodeVector) {
    const size_t nVertices = relativePosition.size();

    nodeVector.resize(nVertices);

    double width = 20;  // TODO:Adapt width depending on how streched the
                        // segment is, i.e., dL/dt

    auto iterNode = nodeVector.begin();
    auto iterRel = relativePosition.cbegin();
    while (iterRel != relativePosition.end()) {
        Time time =
            timeFront - (*iterRel).y * temporalLength;
        Vec2 mid = m_track.position(time);
        Vec2 perp = m_track.direction(time).perpVec() *
                         (width * (*iterRel).x);
        (*iterNode++) = mid + perp;
        ++iterRel;
    }
}

}  // namespace VVipers