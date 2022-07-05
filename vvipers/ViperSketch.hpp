#ifndef VVIPERS_VIPERSKETCH_HPP
#define VVIPERS_VIPERSKETCH_HPP

#include <vector>
#include <vvipers/VectorMath.hpp>

namespace VVipers{

class ViperSketch{
    public:
    static const std::vector<Vec2f>& headNodes() { return getInstance().m_headNodes;}
    static const std::vector<Vec2f>& bodyNodes() { return getInstance().m_bodyNodes;}
    static const std::vector<Vec2f>& tailNodes() { return getInstance().m_tailNodes;}
    private:
    ViperSketch();
    static ViperSketch& getInstance();
    std::vector<Vec2f> m_headNodes;
    std::vector<Vec2f> m_bodyNodes;
    std::vector<Vec2f> m_tailNodes;
};

}

#endif