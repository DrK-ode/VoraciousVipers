#include <fstream>
#include <json/json.h>
#include <vvipers/config.hpp>
#include <vvipers/debug.hpp>
#include <vvipers/Time.hpp>
#include <vvipers/ViperConfig.hpp>

namespace VVipers {

ViperConfig* ViperConfig::m_instance(nullptr);

ViperConfig::ViperConfig( const std::string& viperFile) {
    Json::Value viperValues;
    std::ifstream inputStream(viperFile);
    Json::Reader reader;
    reader.parse(inputStream, viperValues, true);

    auto properties = viperValues["General"];
    m_properties.nominalSpeed =
        properties["nominalSpeed"].asDouble();  // px/s
    m_properties.nominalSegmentLength =
        properties["nominalSegmentLength"].asDouble();  // px
    m_properties.nominalSegmentWidth =
        properties["nominalSegmentWidth"].asDouble();  // px
    m_properties.segmentTemporalLength =
        seconds(m_properties.nominalSegmentLength / m_properties.nominalSpeed);
    m_properties.boostMaxCharge = seconds(properties["boostMaxCharge"].asDouble());  // s
    m_properties.boostRechargeRate =
        properties["boostRechargeRate"].asDouble();  // s per s
    m_properties.boostRechargeCooldown = seconds(
        properties["boostRechargeCooldown"].asDouble());  // s

    Json::Value nodes;
    nodes = viperValues["ViperHead"]["nodes"];
    for (int i = 0; i < nodes.size(); ++i) {
        auto node = nodes[i];
        m_properties.headNodes.push_back(
            {node[0].asDouble(), node[1].asDouble()});
    }
    nodes = viperValues["ViperBody"]["nodes"];
    for (int i = 0; i < nodes.size(); ++i) {
        auto node = nodes[i];
        m_properties.bodyNodes.push_back(
            {node[0].asDouble(), node[1].asDouble()});
    }
    nodes = viperValues["ViperTail"]["nodes"];
    for (int i = 0; i < nodes.size(); ++i) {
        auto node = nodes[i];
        m_properties.tailNodes.push_back(
            {node[0].asDouble(), node[1].asDouble()});
    }

    tagInfo("Read viper properties from ", viperFile );
    logInfo( m_properties );
}

const ViperConfig& ViperConfig::getInstance() {
    if (!m_instance)
        m_instance = new ViperConfig(VIPER_MODEL_FILE_PATH);
    return *m_instance;
}

}  // namespace VVipers