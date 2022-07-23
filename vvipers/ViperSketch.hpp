#ifndef VVIPERS_VIPERSKETCH_HPP
#define VVIPERS_VIPERSKETCH_HPP

#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers{

class ViperSketch{
    public:

    static const std::vector<Vec2>& headNodes() { return getInstance().m_headNodes;}
    static const std::vector<Vec2>& bodyNodes() { return getInstance().m_bodyNodes;}
    static const std::vector<Vec2>& tailNodes() { return getInstance().m_tailNodes;}
    
    private:
    ViperSketch();
    static const ViperSketch& getInstance();
    static ViperSketch* m_instance;

    std::vector<Vec2> m_headNodes;
    std::vector<Vec2> m_bodyNodes;
    std::vector<Vec2> m_tailNodes;
};

}

#endif