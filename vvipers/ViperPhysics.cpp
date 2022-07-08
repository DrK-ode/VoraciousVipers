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
    updateNodes();
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

    updateNodes(headFront, headLength, ViperSketch::headNodes(), m_headNodes);
    updateEdges(m_headNodes, m_headEdges);
    updateNormals(m_headEdges, m_headNormals);
    if (numberOfBodySegments > 0) {
        updateNodes(bodyFront, bodyLength, ViperSketch::bodyNodes(),
                    m_bodyNodes, numberOfBodySegments);
        updateEdges(m_bodyNodes, m_bodyEdges);
        updateNormals(m_bodyEdges, m_bodyNormals);
    }
    updateNodes(tailFront, tailLength, ViperSketch::tailNodes(), m_tailNodes);
    updateEdges(m_tailNodes, m_tailEdges);
    updateNormals(m_tailEdges, m_tailNormals);
}

void ViperPhysics::updateEdges(const std::vector<Vec2>& nodes,
                               std::vector<Vec2>& edges) {
    // Stores the edges in clockwise order starting from the top-right
    edges.resize(nodes.size());
    auto node1 = nodes.cbegin();
    auto node2 = node1 + 1;
    auto edge = edges.begin();
    while (node2 != nodes.end())
        *edge++ = *node2++ - *node1++;
    *edge = nodes.front() - nodes.back();
}

void ViperPhysics::updateNormals(const std::vector<Vec2>& edges,
                                 std::vector<Vec2>& normals) {
    // Only need half the number of normals since the rest are anti-parallel
    normals.resize(edges.size() / 2);
    for (int i = 0; i < normals.size(); ++i)
        normals[i] = edges[i].perpVec();
}

// Helper function since the prepare methods share most of the code
void ViperPhysics::updateNodes(const Time& timeFront,
                               const Time& temporalLength,
                               const std::vector<Vec2>& relativePosition,
                               std::vector<Vec2>& nodeVector,
                               uint32_t nSegments) {
    const size_t nVertPerSeg = relativePosition.size();
    const size_t addPerSeg = nVertPerSeg - 2;
    const size_t nVertices = 2 + addPerSeg * nSegments;
    const Time temporalSegmentLength = temporalLength / nSegments;
    nodeVector.resize(nVertices);

    double width = 20;  // TODO:Adapt width depending on how streched the
                        // segment is, i.e., dL/dt

    auto iterRight = nodeVector.begin();
    auto iterLeft = nodeVector.rbegin();
    /* If each of 2 segment has 6 nodes the first and the last nodes of the
     *second segment would be ignored. The nodes on the right would be saved in
     *positions 0-4 and the ones on the left in positions 5-9.
     **
     **   15 - 10      9 - 0
     **   14   11      8   1
     **   13   12  ->  7   2
     **   24   21      6   3
     **   23 - 22      5 - 4
     */
    auto iterRelRight = relativePosition.cbegin();
    auto iterRelLeft = relativePosition.crbegin();
    int seg = 0;
    // iter and iter.base() actually does not point to the same element
    while (iterRight != iterLeft.base()) {
        while (iterRelRight != iterRelLeft.base()) {
            Time timeRight =
                timeFront - ((seg + (*iterRelRight).y) * temporalSegmentLength);
            Time timeLeft =
                timeFront - ((seg + (*iterRelLeft).y) * temporalSegmentLength);
            Vec2 midRight = m_track.position(timeRight);
            Vec2 midLeft = m_track.position(timeLeft);
            Vec2 perpRight = m_track.direction(timeRight).perpVec() *
                             (width * (*iterRelRight).x);
            Vec2 perpLeft = m_track.direction(timeLeft).perpVec() *
                            (width * (*iterRelLeft).x);
            (*iterRight++) = midRight + perpRight;
            (*iterLeft++) = midLeft + perpLeft;
            ++iterRelRight;
            ++iterRelLeft;
        }
        ++seg;
        iterRelRight = relativePosition.cbegin() + 1;
        iterRelLeft = relativePosition.crbegin() + 1;
    }
}

}  // namespace VVipers