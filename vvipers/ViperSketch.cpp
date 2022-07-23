#include <vvipers/ViperSketch.hpp>
#include <vvipers/config.hpp>
#include <json/json.h>
#include <fstream>
#include <vvipers/debug.hpp>

namespace VVipers {

ViperSketch* ViperSketch::m_instance(nullptr);

ViperSketch::ViperSketch() {
    Json::Value viperValues;
    std::ifstream inputStream(VIPER_MODEL_FILE_PATH);
    Json::Reader reader;
    reader.parse(inputStream, viperValues, true);

    Json::Value nodes;
    nodes = viperValues["ViperHead"]["nodes"];
    for( int i = 0; i < nodes.size(); ++i ){
        auto node = nodes[i];
        m_headNodes.push_back( {node[0].asDouble(), node[1].asDouble()} );
    }
    nodes = viperValues["ViperBody"]["nodes"];
    for( int i = 0; i < nodes.size(); ++i ){
        auto node = nodes[i];
        m_bodyNodes.push_back( {node[0].asDouble(), node[1].asDouble()} );
    }
    nodes = viperValues["ViperTail"]["nodes"];
    for( int i = 0; i < nodes.size(); ++i ){
        auto node = nodes[i];
        m_tailNodes.push_back( {node[0].asDouble(), node[1].asDouble()} );
    }
}

const ViperSketch& ViperSketch::getInstance() {
    if( !m_instance )
        m_instance = new ViperSketch();
    return *m_instance;
}

}  // namespace VVipers