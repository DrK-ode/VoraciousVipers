#ifndef VVIPERS_VIPERCONFIG_HPP
#define VVIPERS_VIPERCONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers {

struct ViperProperties {
    double nominalSpeed;         // px/s
    double nominalSegmentWidth;  // px
    Time boostMaxCharge;         // s
    double boostRechargeRate;    // s per s
    Time boostRechargeCooldown;  // Countdown start

    double headNominalLength;  // px
    Time headDuration;         // s
    std::vector<Vec2> headNodes;

    double bodyNominalLength;  // px
    Time bodyDuration;         // s
    std::vector<Vec2> bodyNodes;

    double tailNominalLength;  // px
    Time tailDuration;         // s
    std::vector<Vec2> tailNodes;
};

std::ostream& operator<<(std::ostream& os, const ViperProperties& vp);

class ViperConfig {
  public:
    static const ViperProperties& properties() {
        return getInstance().m_properties;
    }

  private:
    ViperConfig(const std::string& viperFile);
    static const ViperConfig& getInstance();
    static ViperConfig* m_instance;

    ViperProperties m_properties;
};

}  // namespace VVipers

#endif  // VVIPERS_VIPERCONFIG_HPP
