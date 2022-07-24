#ifndef VVIPERS_VIPERCONFIG_HPP
#define VVIPERS_VIPERCONFIG_HPP

#include <iostream>
#include <string>
#include <vector>
#include <vvipers/Vec2.hpp>

namespace VVipers {

struct ViperProperties {
    double nominalSpeed;          // px/s
    double nominalSegmentLength;  // px
    double nominalSegmentWidth;   // px
    Time segmentTemporalLength;   // s
    Time boostMaxCharge;          // s
    double boostRechargeRate;     // s per s
    Time boostRechargeCooldown;   // Countdown start
    std::vector<Vec2> headNodes;
    std::vector<Vec2> bodyNodes;
    std::vector<Vec2> tailNodes;
};

inline std::ostream& operator<<(std::ostream& os, const ViperProperties& vp){
  os << "Viper properties:" << std::endl;
  os << "  Nominal speed:            " << vp.nominalSpeed << " px/s" << std::endl;
  os << "  Nominal segment length:   " << vp.nominalSegmentLength << " px" << std::endl;
  os << "  Nominal segment width:    " << vp.nominalSegmentWidth << " px" << std::endl;
  os << "  Segment temporal length:  " << vp.segmentTemporalLength << std::endl;
  os << "  Boost max charge:         " << vp.boostMaxCharge << std::endl;
  os << "  Boost recharge rate:      " << vp.boostRechargeRate << " s/s" << std::endl;
  os << "  Boost recharge cooldown:  " << vp.boostRechargeCooldown << std::endl;
  os << "  Number of head nodes:     " << vp.headNodes.size() << std::endl;
  os << "  Number of body nodes:     " << vp.bodyNodes.size() << std::endl;
  os << "  Number of tail nodes:     " << vp.tailNodes.size() << std::endl;
  return os;
}

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

#endif // VVIPERS_VIPERCONFIG_HPP
