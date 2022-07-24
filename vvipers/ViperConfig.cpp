#include <json/json.h>

#include <fstream>
#include <vvipers/Time.hpp>
#include <vvipers/ViperConfig.hpp>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>

namespace VVipers {

ViperConfig* ViperConfig::m_instance(nullptr);

ViperConfig::ViperConfig(const std::string& viperFile) {
    Json::Value viperValues;
    std::ifstream inputStream(viperFile);
    Json::Reader reader;
    reader.parse(inputStream, viperValues, true);

    auto properties = viperValues["General"];
    m_properties.nominalSpeed = properties["nominalSpeed"].asDouble();  // px/s
    m_properties.nominalSegmentWidth =
        properties["nominalSegmentWidth"].asDouble();  // px
    m_properties.boostMaxCharge =
        seconds(properties["boostMaxCharge"].asDouble());  // s
    m_properties.boostRechargeRate =
        properties["boostRechargeRate"].asDouble();  // s per s
    m_properties.boostRechargeCooldown =
        seconds(properties["boostRechargeCooldown"].asDouble());  // s

    Json::Value part;
    Json::Value nodes;
    part = viperValues["ViperHead"];
    m_properties.headNominalLength = part["nominalLength"].asDouble();  // px
    m_properties.headDuration = seconds(m_properties.headNominalLength /
                                        m_properties.nominalSpeed);  // s
    nodes = part["nodes"];
    for (int i = 0; i < nodes.size(); ++i) {
        auto node = nodes[i];
        m_properties.headNodes.push_back(
            {node[0].asDouble(), node[1].asDouble()});
    }

    part = viperValues["ViperBody"];
    m_properties.bodyNominalLength = part["nominalLength"].asDouble();  // px
    m_properties.bodyDuration = seconds(m_properties.bodyNominalLength /
                                        m_properties.nominalSpeed);  // s
    nodes = part["nodes"];
    for (int i = 0; i < nodes.size(); ++i) {
        auto node = nodes[i];
        m_properties.bodyNodes.push_back(
            {node[0].asDouble(), node[1].asDouble()});
    }

    part = viperValues["ViperTail"];
    m_properties.tailNominalLength = part["nominalLength"].asDouble();  // px
    m_properties.tailDuration = seconds(m_properties.tailNominalLength /
                                        m_properties.nominalSpeed);  // s
    nodes = part["nodes"];
    for (int i = 0; i < nodes.size(); ++i) {
        auto node = nodes[i];
        m_properties.tailNodes.push_back(
            {node[0].asDouble(), node[1].asDouble()});
    }

    // tagInfo("Read viper properties from ", viperFile);
    // logInfo(m_properties);
}

const ViperConfig& ViperConfig::getInstance() {
    if (!m_instance)
        m_instance = new ViperConfig(VIPER_MODEL_FILE_PATH);
    return *m_instance;
}

std::ostream& operator<<(std::ostream& os, const ViperProperties& vp) {
    os << "Viper properties:" << std::endl;
    os << "  Nominal speed:            " << vp.nominalSpeed << " px/s"
       << std::endl;
    os << "  Nominal segment width:    " << vp.nominalSegmentWidth << " px"
       << std::endl;
    os << "  Boost max charge:         " << vp.boostMaxCharge << std::endl;
    os << "  Boost recharge rate:      " << vp.boostRechargeRate << " s/s"
       << std::endl;
    os << "  Boost recharge cooldown:  " << vp.boostRechargeCooldown
       << std::endl;
    os << "  Head nominal length:      " << vp.headNominalLength << " px" << std::endl;
    os << "  Body nominal length:      " << vp.bodyNominalLength << " px" << std::endl;
    os << "  Tail nominal length:      " << vp.tailNominalLength << " px" << std::endl;
    os << "  Head duration:            " << vp.headDuration << std::endl;
    os << "  Body duration:            " << vp.bodyDuration << std::endl;
    os << "  Tail duration:            " << vp.tailDuration << std::endl;
    os << "  Number of head nodes:     " << vp.headNodes.size() << std::endl;
    os << "  Number of body nodes:     " << vp.bodyNodes.size() << std::endl;
    os << "  Number of tail nodes:     " << vp.tailNodes.size() << std::endl;
    return os;
}

}  // namespace VVipers