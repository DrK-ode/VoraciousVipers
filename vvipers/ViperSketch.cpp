#include <vvipers/ViperSketch.hpp>

namespace VVipers {

ViperSketch::ViperSketch() {
    m_headNodes.push_back({-0.125, 0});
    m_headNodes.push_back({0.125, 0});
    m_headNodes.push_back({-0.45, 0.625});
    m_headNodes.push_back({0.45, 0.625});
    m_headNodes.push_back({-0.45, 0.8});
    m_headNodes.push_back({0.45, 0.8});
    m_headNodes.push_back({-0.2, 0.95});
    m_headNodes.push_back({0.2, 0.95});
    m_headNodes.push_back({-0.2, 1});
    m_headNodes.push_back({0.2, 1});

    m_bodyNodes.push_back({-0.2, 0});
    m_bodyNodes.push_back({0.2, 0});
    m_bodyNodes.push_back({-0.4, 0.25});
    m_bodyNodes.push_back({0.4, 0.25});
    m_bodyNodes.push_back({-0.4, 0.75});
    m_bodyNodes.push_back({0.4, 0.75});
    m_bodyNodes.push_back({-0.2, 1});
    m_bodyNodes.push_back({0.2, 1});

    m_tailNodes.push_back({-0.2, 0});
    m_tailNodes.push_back({0.2, 0});
    m_tailNodes.push_back({-0.1, 0.5});
    m_tailNodes.push_back({0.1, 0.5});
    m_tailNodes.push_back({-0.01, 1});
    m_tailNodes.push_back({0.01, 1});
}

ViperSketch& ViperSketch::getInstance() {
    static ViperSketch viperSketch;
    return viperSketch;
}

}  // namespace VVipers